// Copyright 2020-2021 Aumoa.lib. All right reserved.

// The teapot model consists of 10 bezier patches. Each patch has 16 control
// points, plus a flag indicating whether it should be mirrored in the Z axis
// as well as in X (all of the teapot is symmetrical from left to right, but
// only some parts are symmetrical from front to back). The control points
// are stored as integer indices into the TeapotControlPoints array.

struct TeapotPatch
{
    bool mirrorZ;
    int indices[16];
};


// Static data array defines the bezier patches that make up the teapot.
const TeapotPatch TeapotPatches[] =
{
    // Rim.
    { true, { 102, 103, 104, 105, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 } },

    // Body.
    { true, { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 } },
    { true, { 24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40 } },

    // Lid.
    { true, { 96, 96, 96, 96, 97, 98, 99, 100, 101, 101, 101, 101, 0, 1, 2, 3 } },
    { true, { 0, 1, 2, 3, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117 } },

    // Handle.
    { false, { 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56 } },
    { false, { 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 28, 65, 66, 67 } },

    // Spout.
    { false, { 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83 } },
    { false, { 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95 } },

    // Bottom.
    { true, { 118, 118, 118, 118, 124, 122, 119, 121, 123, 126, 125, 120, 40, 39, 38, 37 } },
};


// Static array defines the control point positions that make up the teapot.
const Vector3 TeapotControlPoints[] = 
{
    { 0, 0.345f, -0.05f },
    { -0.028f, 0.345f, -0.05f },
    { -0.05f, 0.345f, -0.028f },
    { -0.05f, 0.345f, -0 },
    { 0, 0.3028125f, -0.334375f },
    { -0.18725f, 0.3028125f, -0.334375f },
    { -0.334375f, 0.3028125f, -0.18725f },
    { -0.334375f, 0.3028125f, -0 },
    { 0, 0.3028125f, -0.359375f },
    { -0.20125f, 0.3028125f, -0.359375f },
    { -0.359375f, 0.3028125f, -0.20125f },
    { -0.359375f, 0.3028125f, -0 },
    { 0, 0.27f, -0.375f },
    { -0.21f, 0.27f, -0.375f },
    { -0.375f, 0.27f, -0.21f },
    { -0.375f, 0.27f, -0 },
    { 0, 0.13875f, -0.4375f },
    { -0.245f, 0.13875f, -0.4375f },
    { -0.4375f, 0.13875f, -0.245f },
    { -0.4375f, 0.13875f, -0 },
    { 0, 0.007499993f, -0.5f },
    { -0.28f, 0.007499993f, -0.5f },
    { -0.5f, 0.007499993f, -0.28f },
    { -0.5f, 0.007499993f, -0 },
    { 0, -0.105f, -0.5f },
    { -0.28f, -0.105f, -0.5f },
    { -0.5f, -0.105f, -0.28f },
    { -0.5f, -0.105f, -0 },
    { 0, -0.105f, 0.5f },
    { 0, -0.2175f, -0.5f },
    { -0.28f, -0.2175f, -0.5f },
    { -0.5f, -0.2175f, -0.28f },
    { -0.5f, -0.2175f, -0 },
    { 0, -0.27375f, -0.375f },
    { -0.21f, -0.27375f, -0.375f },
    { -0.375f, -0.27375f, -0.21f },
    { -0.375f, -0.27375f, -0 },
    { 0, -0.2925f, -0.375f },
    { -0.21f, -0.2925f, -0.375f },
    { -0.375f, -0.2925f, -0.21f },
    { -0.375f, -0.2925f, -0 },
    { 0, 0.17625f, 0.4f },
    { -0.075f, 0.17625f, 0.4f },
    { -0.075f, 0.2325f, 0.375f },
    { 0, 0.2325f, 0.375f },
    { 0, 0.17625f, 0.575f },
    { -0.075f, 0.17625f, 0.575f },
    { -0.075f, 0.2325f, 0.625f },
    { 0, 0.2325f, 0.625f },
    { 0, 0.17625f, 0.675f },
    { -0.075f, 0.17625f, 0.675f },
    { -0.075f, 0.2325f, 0.75f },
    { 0, 0.2325f, 0.75f },
    { 0, 0.12f, 0.675f },
    { -0.075f, 0.12f, 0.675f },
    { -0.075f, 0.12f, 0.75f },
    { 0, 0.12f, 0.75f },
    { 0, 0.06375f, 0.675f },
    { -0.075f, 0.06375f, 0.675f },
    { -0.075f, 0.007499993f, 0.75f },
    { 0, 0.007499993f, 0.75f },
    { 0, -0.04875001f, 0.625f },
    { -0.075f, -0.04875001f, 0.625f },
    { -0.075f, -0.09562501f, 0.6625f },
    { 0, -0.09562501f, 0.6625f },
    { -0.075f, -0.105f, 0.5f },
    { -0.075f, -0.18f, 0.475f },
    { 0, -0.18f, 0.475f },
    { 0, 0.02624997f, -0.425f },
    { -0.165f, 0.02624997f, -0.425f },
    { -0.165f, -0.18f, -0.425f },
    { 0, -0.18f, -0.425f },
    { 0, 0.02624997f, -0.65f },
    { -0.165f, 0.02624997f, -0.65f },
    { -0.165f, -0.12375f, -0.775f },
    { 0, -0.12375f, -0.775f },
    { 0, 0.195f, -0.575f },
    { -0.0625f, 0.195f, -0.575f },
    { -0.0625f, 0.17625f, -0.6f },
    { 0, 0.17625f, -0.6f },
    { 0, 0.27f, -0.675f },
    { -0.0625f, 0.27f, -0.675f },
    { -0.0625f, 0.27f, -0.825f },
    { 0, 0.27f, -0.825f },
    { 0, 0.28875f, -0.7f },
    { -0.0625f, 0.28875f, -0.7f },
    { -0.0625f, 0.2934375f, -0.88125f },
    { 0, 0.2934375f, -0.88125f },
    { 0, 0.28875f, -0.725f },
    { -0.0375f, 0.28875f, -0.725f },
    { -0.0375f, 0.298125f, -0.8625f },
    { 0, 0.298125f, -0.8625f },
    { 0, 0.27f, -0.7f },
    { -0.0375f, 0.27f, -0.7f },
    { -0.0375f, 0.27f, -0.8f },
    { 0, 0.27f, -0.8f },
    { 0, 0.4575f, -0 },
    { 0, 0.4575f, -0.2f },
    { -0.1125f, 0.4575f, -0.2f },
    { -0.2f, 0.4575f, -0.1125f },
    { -0.2f, 0.4575f, -0 },
    { 0, 0.3825f, -0 },
    { 0, 0.27f, -0.35f },
    { -0.196f, 0.27f, -0.35f },
    { -0.35f, 0.27f, -0.196f },
    { -0.35f, 0.27f, -0 },
    { 0, 0.3075f, -0.1f },
    { -0.056f, 0.3075f, -0.1f },
    { -0.1f, 0.3075f, -0.056f },
    { -0.1f, 0.3075f, -0 },
    { 0, 0.3075f, -0.325f },
    { -0.182f, 0.3075f, -0.325f },
    { -0.325f, 0.3075f, -0.182f },
    { -0.325f, 0.3075f, -0 },
    { 0, 0.27f, -0.325f },
    { -0.182f, 0.27f, -0.325f },
    { -0.325f, 0.27f, -0.182f },
    { -0.325f, 0.27f, -0 },
    { 0, -0.33f, -0 },
    { -0.1995f, -0.33f, -0.35625f },
    { 0, -0.31125f, -0.375f },
    { 0, -0.33f, -0.35625f },
    { -0.35625f, -0.33f, -0.1995f },
    { -0.375f, -0.31125f, -0 },
    { -0.35625f, -0.33f, -0 },
    { -0.21f, -0.31125f, -0.375f },
    { -0.375f, -0.31125f, -0.21f },
};
