/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2020, assimp team

All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/

#ifndef ASSIMP_BUILD_NO_3MF_IMPORTER

#include "D3MFOpcPackage.h"
#include "Exceptional.h"

#include "XmlParser.h"
#include "ZipArchiveIOSystem.h"
#include "ai_assert.h"
#include "DefaultLogger.hpp"
#include "IOStream.hpp"
#include "IOSystem.hpp"

#include "3MFXmlTags.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <map>
#include <memory>
#include <vector>

namespace Assimp {

namespace D3MF {
// ------------------------------------------------------------------------------------------------

typedef std::shared_ptr<OpcPackageRelationship> OpcPackageRelationshipPtr;

class OpcPackageRelationshipReader {
public:
    OpcPackageRelationshipReader(XmlParser &parser) {
        XmlNode root = parser.getRootNode();
        ParseRootNode(root);
    }

    void ParseRootNode(XmlNode &node) {
        ParseAttributes(node);
        for (XmlNode currentNode = node.first_child(); currentNode; currentNode = currentNode.next_sibling()) {
            std::string name = currentNode.name();
            if (name == "Relationships") {
                ParseRelationsNode(currentNode);
            }
        }
    }

    void ParseAttributes(XmlNode & /*node*/) {
        // empty
    }

    bool validateRels(OpcPackageRelationshipPtr &relPtr) {
        if (relPtr->id.empty() || relPtr->type.empty() || relPtr->target.empty()) {
            return false;
        }
        return true;
    }

    void ParseRelationsNode(XmlNode &node) {
        if (node.empty()) {
            return;
        }

        for (XmlNode currentNode = node.first_child(); currentNode; currentNode = currentNode.next_sibling()) {
            std::string name = currentNode.name();
            if (name == "Relationship") {
                OpcPackageRelationshipPtr relPtr(new OpcPackageRelationship());
                relPtr->id = currentNode.attribute(XmlTag::RELS_ATTRIB_ID.c_str()).as_string();
                relPtr->type = currentNode.attribute(XmlTag::RELS_ATTRIB_TYPE.c_str()).as_string();
                relPtr->target = currentNode.attribute(XmlTag::RELS_ATTRIB_TARGET.c_str()).as_string();
                if (validateRels(relPtr)) {
                    m_relationShips.push_back(relPtr);
                }
            }
        }
    }

    std::vector<OpcPackageRelationshipPtr> m_relationShips;
};

// ------------------------------------------------------------------------------------------------
D3MFOpcPackage::D3MFOpcPackage(IOSystem *pIOHandler, const std::string &rFile) :
        mRootStream(nullptr),
        mZipArchive() {
    mZipArchive.reset(new ZipArchiveIOSystem(pIOHandler, rFile));
    if (!mZipArchive->isOpen()) {
        throw DeadlyImportError("Failed to open file ", rFile, ".");
    }

    std::vector<std::string> fileList;
    mZipArchive->getFileList(fileList);

    for (auto &file : fileList) {
        if (file == D3MF::XmlTag::ROOT_RELATIONSHIPS_ARCHIVE) {
            if (!mZipArchive->Exists(file.c_str())) {
                continue;
            }

            IOStream *fileStream = mZipArchive->Open(file.c_str());
            if (nullptr == fileStream) {
                ai_assert(fileStream != nullptr);
                continue;
            }

            std::string rootFile = ReadPackageRootRelationship(fileStream);
            if (rootFile.size() > 0 && rootFile[0] == '/') {
                rootFile = rootFile.substr(1);
                if (rootFile[0] == '/') {
                    // deal with zip-bug
                    rootFile = rootFile.substr(1);
                }
            }

            ASSIMP_LOG_VERBOSE_DEBUG(rootFile);

            mZipArchive->Close(fileStream);

            mRootStream = mZipArchive->Open(rootFile.c_str());
            ai_assert(mRootStream != nullptr);
            if (nullptr == mRootStream) {
                throw DeadlyExportError("Cannot open root-file in archive : " + rootFile);
            }

        } else if (file == D3MF::XmlTag::CONTENT_TYPES_ARCHIVE) {
            ASSIMP_LOG_WARN_F("Ignored file of unsupported type CONTENT_TYPES_ARCHIVES", file);
        } else {
            ASSIMP_LOG_WARN_F("Ignored file of unknown type: ", file);
        }
    }
}

D3MFOpcPackage::~D3MFOpcPackage() {
    mZipArchive->Close(mRootStream);
}

IOStream *D3MFOpcPackage::RootStream() const {
    return mRootStream;
}

static const std::string ModelRef = "3D/3dmodel.model";

bool D3MFOpcPackage::validate() {
    if (nullptr == mRootStream || nullptr == mZipArchive) {
        return false;
    }

    return mZipArchive->Exists(ModelRef.c_str());
}

std::string D3MFOpcPackage::ReadPackageRootRelationship(IOStream *stream) {
    XmlParser xmlParser;
    if (!xmlParser.parse(stream)) {
        return "";
    }

    OpcPackageRelationshipReader reader(xmlParser);

    auto itr = std::find_if(reader.m_relationShips.begin(), reader.m_relationShips.end(), [](const OpcPackageRelationshipPtr &rel) {
        return rel->type == XmlTag::PACKAGE_START_PART_RELATIONSHIP_TYPE;
    });

    if (itr == reader.m_relationShips.end()) {
        throw DeadlyImportError("Cannot find ", XmlTag::PACKAGE_START_PART_RELATIONSHIP_TYPE);
    }

    return (*itr)->target;
}

} // Namespace D3MF
} // Namespace Assimp

#endif //ASSIMP_BUILD_NO_3MF_IMPORTER
