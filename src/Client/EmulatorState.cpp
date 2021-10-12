#include "Client/State.h"
#include "Client/EmulatorState.h"
#include "Common/Keys.h"
#include "Common/WaveForm.h"

#include <vector>
#include <iostream>

#define USE_VGA

using namespace Client;

SystemIO::SystemIO() : cursor(0,0) {
    screenbuffer.resize(lines);
    screenbuffer[cursor.Y()][cursor.X()] = (char)228;

    screen.fill(Common::Colour::DarkSlateBlue.RGBA());

#ifdef USE_VGA
    colourLookup[0] = Common::Colour(0, 0, 0); //Black #000000
    colourLookup[1] = Common::Colour(128, 0, 0); //Maroon #800000
    colourLookup[2] = Common::Colour(0, 128, 0); //Green #008000
    colourLookup[3] = Common::Colour(128, 128, 0); //Olive #808000
    colourLookup[4] = Common::Colour(0, 0, 128); //Navy #000080
    colourLookup[5] = Common::Colour(128, 0, 128); //Purple #800080
    colourLookup[6] = Common::Colour(0, 128, 128); //Teal #008080
    colourLookup[7] = Common::Colour(192, 192, 192); //Silver #c0c0c0
    colourLookup[8] = Common::Colour(128, 128, 128); //Grey #808080
    colourLookup[9] = Common::Colour(255, 0, 0); //Red #ff0000
    colourLookup[10] = Common::Colour(0, 255, 0); //Lime #00ff00
    colourLookup[11] = Common::Colour(255, 255, 0); //Yellow #ffff00
    colourLookup[12] = Common::Colour(0, 0, 255); //Blue #0000ff
    colourLookup[13] = Common::Colour(255, 0, 255); //Fuchsia #ff00ff
    colourLookup[14] = Common::Colour(0, 255, 255); //Aqua #00ffff
    colourLookup[15] = Common::Colour(255, 255, 255); //White #ffffff
    colourLookup[16] = Common::Colour(0, 0, 0); //Grey0 #000000
    colourLookup[17] = Common::Colour(0, 0, 95); //NavyBlue #00005f
    colourLookup[18] = Common::Colour(0, 0, 135); //DarkBlue #000087
    colourLookup[19] = Common::Colour(0, 0, 175); //Blue3 #0000af
    colourLookup[20] = Common::Colour(0, 0, 215); //Blue3 #0000d7
    colourLookup[21] = Common::Colour(0, 0, 255); //Blue1 #0000ff
    colourLookup[22] = Common::Colour(0, 95, 0); //DarkGreen #005f00
    colourLookup[23] = Common::Colour(0, 95, 95); //DeepSkyBlue4 #005f5f
    colourLookup[24] = Common::Colour(0, 95, 135); //DeepSkyBlue4 #005f87
    colourLookup[25] = Common::Colour(0, 95, 175); //DeepSkyBlue4 #005faf
    colourLookup[26] = Common::Colour(0, 95, 215); //DodgerBlue3 #005fd7
    colourLookup[27] = Common::Colour(0, 95, 255); //DodgerBlue2 #005fff
    colourLookup[28] = Common::Colour(0, 135, 0); //Green4 #008700
    colourLookup[29] = Common::Colour(0, 135, 95); //SpringGreen4 #00875f
    colourLookup[30] = Common::Colour(0, 135, 135); //Turquoise4 #008787
    colourLookup[31] = Common::Colour(0, 135, 175); //DeepSkyBlue3 #0087af
    colourLookup[32] = Common::Colour(0, 135, 215); //DeepSkyBlue3 #0087d7
    colourLookup[33] = Common::Colour(0, 135, 255); //DodgerBlue1 #0087ff
    colourLookup[34] = Common::Colour(0, 175, 0); //Green3 #00af00
    colourLookup[35] = Common::Colour(0, 175, 95); //SpringGreen3 #00af5f
    colourLookup[36] = Common::Colour(0, 175, 135); //DarkCyan #00af87
    colourLookup[37] = Common::Colour(0, 175, 175); //LightSeaGreen #00afaf
    colourLookup[38] = Common::Colour(0, 175, 215); //DeepSkyBlue2 #00afd7
    colourLookup[39] = Common::Colour(0, 175, 255); //DeepSkyBlue1 #00afff
    colourLookup[40] = Common::Colour(0, 215, 0); //Green3 #00d700
    colourLookup[41] = Common::Colour(0, 215, 95); //SpringGreen3 #00d75f
    colourLookup[42] = Common::Colour(0, 215, 135); //SpringGreen2 #00d787
    colourLookup[43] = Common::Colour(0, 215, 175); //Cyan3 #00d7af
    colourLookup[44] = Common::Colour(0, 215, 215); //DarkTurquoise #00d7d7
    colourLookup[45] = Common::Colour(0, 215, 255); //Turquoise2 #00d7ff
    colourLookup[46] = Common::Colour(0, 255, 0); //Green1 #00ff00
    colourLookup[47] = Common::Colour(0, 255, 95); //SpringGreen2 #00ff5f
    colourLookup[48] = Common::Colour(0, 255, 135); //SpringGreen1 #00ff87
    colourLookup[49] = Common::Colour(0, 255, 175); //MediumSpringGreen #00ffaf
    colourLookup[50] = Common::Colour(0, 255, 215); //Cyan2 #00ffd7
    colourLookup[51] = Common::Colour(0, 255, 255); //Cyan1 #00ffff
    colourLookup[52] = Common::Colour(95, 0, 0); //DarkRed #5f0000
    colourLookup[53] = Common::Colour(95, 0, 95); //DeepPink4 #5f005f
    colourLookup[54] = Common::Colour(95, 0, 135); //Purple4 #5f0087
    colourLookup[55] = Common::Colour(95, 0, 175); //Purple4 #5f00af
    colourLookup[56] = Common::Colour(95, 0, 215); //Purple3 #5f00d7
    colourLookup[57] = Common::Colour(95, 0, 255); //BlueViolet #5f00ff
    colourLookup[58] = Common::Colour(95, 95, 0); //Orange4 #5f5f00
    colourLookup[59] = Common::Colour(95, 95, 95); //Grey37 #5f5f5f
    colourLookup[60] = Common::Colour(95, 95, 135); //MediumPurple4 #5f5f87
    colourLookup[61] = Common::Colour(95, 95, 175); //SlateBlue3 #5f5faf
    colourLookup[62] = Common::Colour(95, 95, 215); //SlateBlue3 #5f5fd7
    colourLookup[63] = Common::Colour(95, 95, 255); //RoyalBlue1 #5f5fff
    colourLookup[64] = Common::Colour(95, 135, 0); //Chartreuse4 #5f8700
    colourLookup[65] = Common::Colour(95, 135, 95); //DarkSeaGreen4 #5f875f
    colourLookup[66] = Common::Colour(95, 135, 135); //PaleTurquoise4 #5f8787
    colourLookup[67] = Common::Colour(95, 135, 175); //SteelBlue #5f87af
    colourLookup[68] = Common::Colour(95, 135, 215); //SteelBlue3 #5f87d7
    colourLookup[69] = Common::Colour(95, 135, 255); //CornflowerBlue #5f87ff
    colourLookup[70] = Common::Colour(95, 175, 0); //Chartreuse3 #5faf00
    colourLookup[71] = Common::Colour(95, 175, 95); //DarkSeaGreen4 #5faf5f
    colourLookup[72] = Common::Colour(95, 175, 135); //CadetBlue #5faf87
    colourLookup[73] = Common::Colour(95, 175, 175); //CadetBlue #5fafaf
    colourLookup[74] = Common::Colour(95, 175, 215); //SkyBlue3 #5fafd7
    colourLookup[75] = Common::Colour(95, 175, 255); //SteelBlue1 #5fafff
    colourLookup[76] = Common::Colour(95, 215, 0); //Chartreuse3 #5fd700
    colourLookup[77] = Common::Colour(95, 215, 95); //PaleGreen3 #5fd75f
    colourLookup[78] = Common::Colour(95, 215, 135); //SeaGreen3 #5fd787
    colourLookup[79] = Common::Colour(95, 215, 175); //Aquamarine3 #5fd7af
    colourLookup[80] = Common::Colour(95, 215, 215); //MediumTurquoise #5fd7d7
    colourLookup[81] = Common::Colour(95, 215, 255); //SteelBlue1 #5fd7ff
    colourLookup[82] = Common::Colour(95, 255, 0); //Chartreuse2 #5fff00
    colourLookup[83] = Common::Colour(95, 255, 95); //SeaGreen2 #5fff5f
    colourLookup[84] = Common::Colour(95, 255, 135); //SeaGreen1 #5fff87
    colourLookup[85] = Common::Colour(95, 255, 175); //SeaGreen1 #5fffaf
    colourLookup[86] = Common::Colour(95, 255, 215); //Aquamarine1 #5fffd7
    colourLookup[87] = Common::Colour(95, 255, 255); //DarkSlateGray2 #5fffff
    colourLookup[88] = Common::Colour(135, 0, 0); //DarkRed #870000
    colourLookup[89] = Common::Colour(135, 0, 95); //DeepPink4 #87005f
    colourLookup[90] = Common::Colour(135, 0, 135); //DarkMagenta #870087
    colourLookup[91] = Common::Colour(135, 0, 175); //DarkMagenta #8700af
    colourLookup[92] = Common::Colour(135, 0, 215); //DarkViolet #8700d7
    colourLookup[93] = Common::Colour(135, 0, 255); //Purple #8700ff
    colourLookup[94] = Common::Colour(135, 95, 0); //Orange4 #875f00
    colourLookup[95] = Common::Colour(135, 95, 95); //LightPink4 #875f5f
    colourLookup[96] = Common::Colour(135, 95, 135); //Plum4 #875f87
    colourLookup[97] = Common::Colour(135, 95, 175); //MediumPurple3 #875faf
    colourLookup[98] = Common::Colour(135, 95, 215); //MediumPurple3 #875fd7
    colourLookup[99] = Common::Colour(135, 95, 255); //SlateBlue1 #875fff
    colourLookup[100] = Common::Colour(135, 135, 0); //Yellow4 #878700
    colourLookup[101] = Common::Colour(135, 135, 95); //Wheat4 #87875f
    colourLookup[102] = Common::Colour(135, 135, 135); //Grey53 #878787
    colourLookup[103] = Common::Colour(135, 135, 175); //LightSlateGrey #8787af
    colourLookup[104] = Common::Colour(135, 135, 215); //MediumPurple #8787d7
    colourLookup[105] = Common::Colour(135, 135, 255); //LightSlateBlue #8787ff
    colourLookup[106] = Common::Colour(135, 175, 0); //Yellow4 #87af00
    colourLookup[107] = Common::Colour(135, 175, 95); //DarkOliveGreen3 #87af5f
    colourLookup[108] = Common::Colour(135, 175, 135); //DarkSeaGreen #87af87
    colourLookup[109] = Common::Colour(135, 175, 175); //LightSkyBlue3 #87afaf
    colourLookup[110] = Common::Colour(135, 175, 215); //LightSkyBlue3 #87afd7
    colourLookup[111] = Common::Colour(135, 175, 255); //SkyBlue2 #87afff
    colourLookup[112] = Common::Colour(135, 215, 0); //Chartreuse2 #87d700
    colourLookup[113] = Common::Colour(135, 215, 95); //DarkOliveGreen3 #87d75f
    colourLookup[114] = Common::Colour(135, 215, 135); //PaleGreen3 #87d787
    colourLookup[115] = Common::Colour(135, 215, 175); //DarkSeaGreen3 #87d7af
    colourLookup[116] = Common::Colour(135, 215, 215); //DarkSlateGray3 #87d7d7
    colourLookup[117] = Common::Colour(135, 215, 255); //SkyBlue1 #87d7ff
    colourLookup[118] = Common::Colour(135, 255, 0); //Chartreuse1 #87ff00
    colourLookup[119] = Common::Colour(135, 255, 95); //LightGreen #87ff5f
    colourLookup[120] = Common::Colour(135, 255, 135); //LightGreen #87ff87
    colourLookup[121] = Common::Colour(135, 255, 175); //PaleGreen1 #87ffaf
    colourLookup[122] = Common::Colour(135, 255, 215); //Aquamarine1 #87ffd7
    colourLookup[123] = Common::Colour(135, 255, 255); //DarkSlateGray1 #87ffff
    colourLookup[124] = Common::Colour(175, 0, 0); //Red3 #af0000
    colourLookup[125] = Common::Colour(175, 0, 95); //DeepPink4 #af005f
    colourLookup[126] = Common::Colour(175, 0, 135); //MediumVioletRed #af0087
    colourLookup[127] = Common::Colour(175, 0, 175); //Magenta3 #af00af
    colourLookup[128] = Common::Colour(175, 0, 215); //DarkViolet #af00d7
    colourLookup[129] = Common::Colour(175, 0, 255); //Purple #af00ff
    colourLookup[130] = Common::Colour(175, 95, 0); //DarkOrange3 #af5f00
    colourLookup[131] = Common::Colour(175, 95, 95); //IndianRed #af5f5f
    colourLookup[132] = Common::Colour(175, 95, 135); //HotPink3 #af5f87
    colourLookup[133] = Common::Colour(175, 95, 175); //MediumOrchid3 #af5faf
    colourLookup[134] = Common::Colour(175, 95, 215); //MediumOrchid #af5fd7
    colourLookup[135] = Common::Colour(175, 95, 255); //MediumPurple2 #af5fff
    colourLookup[136] = Common::Colour(175, 135, 0); //DarkGoldenrod #af8700
    colourLookup[137] = Common::Colour(175, 135, 95); //LightSalmon3 #af875f
    colourLookup[138] = Common::Colour(175, 135, 135); //RosyBrown #af8787
    colourLookup[139] = Common::Colour(175, 135, 175); //Grey63 #af87af
    colourLookup[140] = Common::Colour(175, 135, 215); //MediumPurple2 #af87d7
    colourLookup[141] = Common::Colour(175, 135, 255); //MediumPurple1 #af87ff
    colourLookup[142] = Common::Colour(175, 175, 0); //Gold3 #afaf00
    colourLookup[143] = Common::Colour(175, 175, 95); //DarkKhaki #afaf5f
    colourLookup[144] = Common::Colour(175, 175, 135); //NavajoWhite3 #afaf87
    colourLookup[145] = Common::Colour(175, 175, 175); //Grey69 #afafaf
    colourLookup[146] = Common::Colour(175, 175, 215); //LightSteelBlue3 #afafd7
    colourLookup[147] = Common::Colour(175, 175, 255); //LightSteelBlue #afafff
    colourLookup[148] = Common::Colour(175, 215, 0); //Yellow3 #afd700
    colourLookup[149] = Common::Colour(175, 215, 95); //DarkOliveGreen3 #afd75f
    colourLookup[150] = Common::Colour(175, 215, 135); //DarkSeaGreen3 #afd787
    colourLookup[151] = Common::Colour(175, 215, 175); //DarkSeaGreen2 #afd7af
    colourLookup[152] = Common::Colour(175, 215, 215); //LightCyan3 #afd7d7
    colourLookup[153] = Common::Colour(175, 215, 255); //LightSkyBlue1 #afd7ff
    colourLookup[154] = Common::Colour(175, 255, 0); //GreenYellow #afff00
    colourLookup[155] = Common::Colour(175, 255, 95); //DarkOliveGreen2 #afff5f
    colourLookup[156] = Common::Colour(175, 255, 135); //PaleGreen1 #afff87
    colourLookup[157] = Common::Colour(175, 255, 175); //DarkSeaGreen2 #afffaf
    colourLookup[158] = Common::Colour(175, 255, 215); //DarkSeaGreen1 #afffd7
    colourLookup[159] = Common::Colour(175, 255, 255); //PaleTurquoise1 #afffff
    colourLookup[160] = Common::Colour(215, 0, 0); //Red3 #d70000
    colourLookup[161] = Common::Colour(215, 0, 95); //DeepPink3 #d7005f
    colourLookup[162] = Common::Colour(215, 0, 135); //DeepPink3 #d70087
    colourLookup[163] = Common::Colour(215, 0, 175); //Magenta3 #d700af
    colourLookup[164] = Common::Colour(215, 0, 215); //Magenta3 #d700d7
    colourLookup[165] = Common::Colour(215, 0, 255); //Magenta2 #d700ff
    colourLookup[166] = Common::Colour(215, 95, 0); //DarkOrange3 #d75f00
    colourLookup[167] = Common::Colour(215, 95, 95); //IndianRed #d75f5f
    colourLookup[168] = Common::Colour(215, 95, 135); //HotPink3 #d75f87
    colourLookup[169] = Common::Colour(215, 95, 175); //HotPink2 #d75faf
    colourLookup[170] = Common::Colour(215, 95, 215); //Orchid #d75fd7
    colourLookup[171] = Common::Colour(215, 95, 255); //MediumOrchid1 #d75fff
    colourLookup[172] = Common::Colour(215, 135, 0); //Orange3 #d78700
    colourLookup[173] = Common::Colour(215, 135, 95); //LightSalmon3 #d7875f
    colourLookup[174] = Common::Colour(215, 135, 135); //LightPink3 #d78787
    colourLookup[175] = Common::Colour(215, 135, 175); //Pink3 #d787af
    colourLookup[176] = Common::Colour(215, 135, 215); //Plum3 #d787d7
    colourLookup[177] = Common::Colour(215, 135, 255); //Violet #d787ff
    colourLookup[178] = Common::Colour(215, 175, 0); //Gold3 #d7af00
    colourLookup[179] = Common::Colour(215, 175, 95); //LightGoldenrod3 #d7af5f
    colourLookup[180] = Common::Colour(215, 175, 135); //Tan #d7af87
    colourLookup[181] = Common::Colour(215, 175, 175); //MistyRose3 #d7afaf
    colourLookup[182] = Common::Colour(215, 175, 215); //Thistle3 #d7afd7
    colourLookup[183] = Common::Colour(215, 175, 255); //Plum2 #d7afff
    colourLookup[184] = Common::Colour(215, 215, 0); //Yellow3 #d7d700
    colourLookup[185] = Common::Colour(215, 215, 95); //Khaki3 #d7d75f
    colourLookup[186] = Common::Colour(215, 215, 135); //LightGoldenrod2 #d7d787
    colourLookup[187] = Common::Colour(215, 215, 175); //LightYellow3 #d7d7af
    colourLookup[188] = Common::Colour(215, 215, 215); //Grey84 #d7d7d7
    colourLookup[189] = Common::Colour(215, 215, 255); //LightSteelBlue1 #d7d7ff
    colourLookup[190] = Common::Colour(215, 255, 0); //Yellow2 #d7ff00
    colourLookup[191] = Common::Colour(215, 255, 95); //DarkOliveGreen1 #d7ff5f
    colourLookup[192] = Common::Colour(215, 255, 135); //DarkOliveGreen1 #d7ff87
    colourLookup[193] = Common::Colour(215, 255, 175); //DarkSeaGreen1 #d7ffaf
    colourLookup[194] = Common::Colour(215, 255, 215); //Honeydew2 #d7ffd7
    colourLookup[195] = Common::Colour(215, 255, 255); //LightCyan1 #d7ffff
    colourLookup[196] = Common::Colour(255, 0, 0); //Red1 #ff0000
    colourLookup[197] = Common::Colour(255, 0, 95); //DeepPink2 #ff005f
    colourLookup[198] = Common::Colour(255, 0, 135); //DeepPink1 #ff0087
    colourLookup[199] = Common::Colour(255, 0, 175); //DeepPink1 #ff00af
    colourLookup[200] = Common::Colour(255, 0, 215); //Magenta2 #ff00d7
    colourLookup[201] = Common::Colour(255, 0, 255); //Magenta1 #ff00ff
    colourLookup[202] = Common::Colour(255, 95, 0); //OrangeRed1 #ff5f00
    colourLookup[203] = Common::Colour(255, 95, 95); //IndianRed1 #ff5f5f
    colourLookup[204] = Common::Colour(255, 95, 135); //IndianRed1 #ff5f87
    colourLookup[205] = Common::Colour(255, 95, 175); //HotPink #ff5faf
    colourLookup[206] = Common::Colour(255, 95, 215); //HotPink #ff5fd7
    colourLookup[207] = Common::Colour(255, 95, 255); //MediumOrchid1 #ff5fff
    colourLookup[208] = Common::Colour(255, 135, 0); //DarkOrange #ff8700
    colourLookup[209] = Common::Colour(255, 135, 95); //Salmon1 #ff875f
    colourLookup[210] = Common::Colour(255, 135, 135); //LightCoral #ff8787
    colourLookup[211] = Common::Colour(255, 135, 175); //PaleVioletRed1 #ff87af
    colourLookup[212] = Common::Colour(255, 135, 215); //Orchid2 #ff87d7
    colourLookup[213] = Common::Colour(255, 135, 255); //Orchid1 #ff87ff
    colourLookup[214] = Common::Colour(255, 175, 0); //Orange1 #ffaf00
    colourLookup[215] = Common::Colour(255, 175, 95); //SandyBrown #ffaf5f
    colourLookup[216] = Common::Colour(255, 175, 135); //LightSalmon1 #ffaf87
    colourLookup[217] = Common::Colour(255, 175, 175); //LightPink1 #ffafaf
    colourLookup[218] = Common::Colour(255, 175, 215); //Pink1 #ffafd7
    colourLookup[219] = Common::Colour(255, 175, 255); //Plum1 #ffafff
    colourLookup[220] = Common::Colour(255, 215, 0); //Gold1 #ffd700
    colourLookup[221] = Common::Colour(255, 215, 95); //LightGoldenrod2 #ffd75f
    colourLookup[222] = Common::Colour(255, 215, 135); //LightGoldenrod2 #ffd787
    colourLookup[223] = Common::Colour(255, 215, 175); //NavajoWhite1 #ffd7af
    colourLookup[224] = Common::Colour(255, 215, 215); //MistyRose1 #ffd7d7
    colourLookup[225] = Common::Colour(255, 215, 255); //Thistle1 #ffd7ff
    colourLookup[226] = Common::Colour(255, 255, 0); //Yellow1 #ffff00
    colourLookup[227] = Common::Colour(255, 255, 95); //LightGoldenrod1 #ffff5f
    colourLookup[228] = Common::Colour(255, 255, 135); //Khaki1 #ffff87
    colourLookup[229] = Common::Colour(255, 255, 175); //Wheat1 #ffffaf
    colourLookup[230] = Common::Colour(255, 255, 215); //Cornsilk1 #ffffd7
    colourLookup[231] = Common::Colour(255, 255, 255); //Grey100 #ffffff
    colourLookup[232] = Common::Colour(8, 8, 8); //Grey3 #080808
    colourLookup[233] = Common::Colour(18, 18, 18); //Grey7 #121212
    colourLookup[234] = Common::Colour(28, 28, 28); //Grey11 #1c1c1c
    colourLookup[235] = Common::Colour(38, 38, 38); //Grey15 #262626
    colourLookup[236] = Common::Colour(48, 48, 48); //Grey19 #303030
    colourLookup[237] = Common::Colour(58, 58, 58); //Grey23 #3a3a3a
    colourLookup[238] = Common::Colour(68, 68, 68); //Grey27 #444444
    colourLookup[239] = Common::Colour(78, 78, 78); //Grey30 #4e4e4e
    colourLookup[240] = Common::Colour(88, 88, 88); //Grey35 #585858
    colourLookup[241] = Common::Colour(98, 98, 98); //Grey39 #626262
    colourLookup[242] = Common::Colour(108, 108, 108); //Grey42 #6c6c6c
    colourLookup[243] = Common::Colour(118, 118, 118); //Grey46 #767676
    colourLookup[244] = Common::Colour(128, 128, 128); //Grey50 #808080
    colourLookup[245] = Common::Colour(138, 138, 138); //Grey54 #8a8a8a
    colourLookup[246] = Common::Colour(148, 148, 148); //Grey58 #949494
    colourLookup[247] = Common::Colour(158, 158, 158); //Grey62 #9e9e9e
    colourLookup[248] = Common::Colour(168, 168, 168); //Grey66 #a8a8a8
    colourLookup[249] = Common::Colour(178, 178, 178); //Grey70 #b2b2b2
    colourLookup[250] = Common::Colour(188, 188, 188); //Grey74 #bcbcbc
    colourLookup[251] = Common::Colour(198, 198, 198); //Grey78 #c6c6c6
    colourLookup[252] = Common::Colour(208, 208, 208); //Grey82 #d0d0d0
    colourLookup[253] = Common::Colour(218, 218, 218); //Grey85 #dadada
    colourLookup[254] = Common::Colour(228, 228, 228); //Grey89 #e4e4e4
    colourLookup[255] = Common::Colour(238, 238, 238); //Grey93 #eeeeee
#else
    for (size_t i = 0; i < colourLookup.size(); i++) {
        colourLookup[i] = Common::Colour::Colour8(i);
    }
#endif

}

void SystemIO::cls() {
    screen.fill(Common::Colour::DarkSlateBlue.RGBA());
    cursor = Point(0,0);
    screenbuffer.clear();
    screenbuffer.resize(lines);
}

void SystemIO::setpixel(uint16_t x, uint16_t y, uint8_t pixel) {
    screen[y*320 + x] = colourLookup[pixel].RGBA();
}

uint8_t SystemIO::getpixel(uint16_t x, uint16_t y) {
    return Common::Colour(screen[y*320 + x]).RGB332();
}

void SystemIO::write(uint8_t c) {
    char chr = (char)c;

    if (chr == '\n') {
        cursor = (cursor + Point(0,1)).withX(0);
        chr = 0;
    } else if (chr == 8) {
        if (cursor != Point(0,0)) {
            cursor = (cursor - Point(1,0));
            screenbuffer[cursor.Y()][cursor.X()] = ' ';
        }
        return;
    } else if (chr == '\t') {
        write(' ');
        write(' ');
        return;
    }

    if (cursor.X() >= chars)
        cursor = (cursor + Point(0,1)).withX(0);

    if (cursor.Y() >= lines) {
        cursor = cursor - Point(0, 1);
        screenbuffer.erase(screenbuffer.begin());
        screenbuffer.push_back(std::array<char, chars+1>());
    }

    if (chr) {
        screenbuffer[cursor.Y()][cursor.X()] = chr;
        cursor = (cursor + Point(1,0));
    } else {
        screenbuffer[cursor.Y()][cursor.X()] = (char)228;
    }
}

uint8_t SystemIO::read(bool noecho) {
    if (inputBuffer.size() == 0)
        return 0;

    uint8_t c = (uint8_t)inputBuffer.front();

    if (!noecho)
        write(c);

    inputBuffer.pop();

    return c;
}

void SystemIO::puts(const std::string &str) {
    for (auto c : str) {
        write(c);
    }
}

std::string SystemIO::gets() {
    return "";
}

void SystemIO::blit(uint16_t x, uint16_t y, std::vector<uint8_t> buffer) {
    uint32_t *ptr = screen.data();
    ptr += y*320 + x;

    std::memcpy(ptr, buffer.data(), buffer.size());
}

void SystemIO::sound(uint8_t voice, float frequency, uint16_t duration) {
    auto voiceConfig = voices[voice];

    soundBuffer.push(SoundBufferObject(voice, frequency, duration, voiceConfig));
}

void SystemIO::voice(uint8_t voice, uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release) {
    voices[voice] = VoiceConfig(waveForm, volume, attack, decay, sustain, release);
}

void EmulatorState::onRender(State *state, const uint32_t time) {
    state->getRenderer()->drawBuffer(sysio->getScreen().data(), 320, 240);

    uint16_t lineoffset = 0;
    for (auto line : sysio->getScreenBuffer()) {
        state->getRenderer()->drawString(0, lineoffset*8, 8, 8, std::string(line.data()), Common::Colour::MediumPurple);
        lineoffset++;
    }
}

static std::string str_toupper(std::string s) {
    std::transform(
        s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::toupper(c); }
    );

    return s;
}

void EmulatorState::onTick(State *state, const uint32_t time) {
    static bool done = false;
    static std::string input = "";
    static std::shared_ptr<Emulator::Debugger> debugger = std::make_shared<Emulator::Debugger>();

    if (!done) {
        try {
            done = vm->run(std::dynamic_pointer_cast<Emulator::SysIO>(sysio), *program, clockspeed, debug ? debugger : NULL);
        } catch (const std::runtime_error &re) {
            sysio->puts(std::string("Runtime Error: ") + re.what() + std::string("\n"));
            done = true;
            return;
        }

        std::optional<SoundBufferObject> s = sysio->nextSound();
        while (s != std::nullopt) {
            auto sound = *s;
            auto voice = sysio->getVoice(sound.voice);
            state->getSys()->sound(sound.voice, sound.frequency, sound.duration, voice.waveForm, voice.volume, voice.attack, voice.decay, voice.sustain, voice.release);
            s = sysio->nextSound();
        }

        return;
    }

    while (auto c = sysio->buffer()) {
        sysio->write(c);

        if (c == '\n') {
            auto cmd = str_toupper(input);
            if (cmd == "LIST") {
                for (auto const &b : basic) {
                    sysio->puts(std::to_string(b.first));
                    for (auto token : b.second) {
                        sysio->puts(std::string(" ") + token.toString());
                    }
                    sysio->write('\n');
                }
            } else if (cmd == "RUN") {
                auto run = program->add(Emulator::OpCode::NOP);
                try {
                    compile(basic, *program);
                    vm->Jump(run);
                } catch (const std::invalid_argument &ia) {
                    sysio->puts(ia.what() + std::string("\n"));
                } catch (const std::domain_error &de) {
                    sysio->puts(de.what() + std::string("\n"));
                }
                done = false;
            } else if (cmd == "CLS") {
                sysio->cls();
            } else if (cmd == "CLEAR") {
                basic.clear();
            } else {
                auto basicline = parseLine(input);
                if (basicline.first) {
                    basic[basicline.first] = basicline.second;
                } else {
                    sysio->puts("? Unknown command\n");
                }
            }

            input = "";
        } else if (c == 8) {
            input.pop_back();
        } else {
            input += c;
        }
    }
}

void EmulatorState::onMouseMove(State *state, const MouseMove &event) {
}

void EmulatorState::onMouseButtonPress(State *state, const MouseClick &event) {
}

void EmulatorState::onMouseButtonRelease(State *state, const MouseClick &event) {
}

void EmulatorState::onKeyDown(State *state, const KeyPress &event) {
    if (event.keyCode == Common::Keys::A) {
        sysio->buffer(event.shiftMod ? 'A' : 'a');
    } else if (event.keyCode == Common::Keys::B) {
        sysio->buffer(event.shiftMod ? 'B' : 'b');
    } else if (event.keyCode == Common::Keys::C) {
        sysio->buffer(event.shiftMod ? 'C' : 'c');
    } else if (event.keyCode == Common::Keys::D) {
        sysio->buffer(event.shiftMod ? 'D' : 'd');
    } else if (event.keyCode == Common::Keys::E) {
        sysio->buffer(event.shiftMod ? 'E' : 'e');
    } else if (event.keyCode == Common::Keys::F) {
        sysio->buffer(event.shiftMod ? 'F' : 'f');
    } else if (event.keyCode == Common::Keys::G) {
        sysio->buffer(event.shiftMod ? 'G' : 'g');
    } else if (event.keyCode == Common::Keys::H) {
        sysio->buffer(event.shiftMod ? 'H' : 'h');
    } else if (event.keyCode == Common::Keys::I) {
        sysio->buffer(event.shiftMod ? 'I' : 'i');
    } else if (event.keyCode == Common::Keys::J) {
        sysio->buffer(event.shiftMod ? 'J' : 'j');
    } else if (event.keyCode == Common::Keys::K) {
        sysio->buffer(event.shiftMod ? 'K' : 'k');
    } else if (event.keyCode == Common::Keys::L) {
        sysio->buffer(event.shiftMod ? 'L' : 'l');
    } else if (event.keyCode == Common::Keys::M) {
        sysio->buffer(event.shiftMod ? 'M' : 'm');
    } else if (event.keyCode == Common::Keys::N) {
        sysio->buffer(event.shiftMod ? 'N' : 'n');
    } else if (event.keyCode == Common::Keys::O) {
        sysio->buffer(event.shiftMod ? 'O' : 'o');
    } else if (event.keyCode == Common::Keys::P) {
        sysio->buffer(event.shiftMod ? 'P' : 'p');
    } else if (event.keyCode == Common::Keys::Q) {
        sysio->buffer(event.shiftMod ? 'Q' : 'q');
    } else if (event.keyCode == Common::Keys::R) {
        sysio->buffer(event.shiftMod ? 'R' : 'r');
    } else if (event.keyCode == Common::Keys::S) {
        sysio->buffer(event.shiftMod ? 'S' : 's');
    } else if (event.keyCode == Common::Keys::T) {
        sysio->buffer(event.shiftMod ? 'T' : 't');
    } else if (event.keyCode == Common::Keys::U) {
        sysio->buffer(event.shiftMod ? 'U' : 'u');
    } else if (event.keyCode == Common::Keys::V) {
        sysio->buffer(event.shiftMod ? 'V' : 'v');
    } else if (event.keyCode == Common::Keys::W) {
        sysio->buffer(event.shiftMod ? 'W' : 'w');
    } else if (event.keyCode == Common::Keys::X) {
        sysio->buffer(event.shiftMod ? 'X' : 'x');
    } else if (event.keyCode == Common::Keys::Y) {
        sysio->buffer(event.shiftMod ? 'Y' : 'y');
    } else if (event.keyCode == Common::Keys::Z) {
        sysio->buffer(event.shiftMod ? 'Z' : 'z');
    } else if (event.keyCode == Common::Keys::Num1) {
        sysio->buffer(event.shiftMod ? '!' : '1');
    } else if (event.keyCode == Common::Keys::Num2) {
        sysio->buffer(event.shiftMod ? '@' : '2');
    } else if (event.keyCode == Common::Keys::Num3) {
        sysio->buffer(event.shiftMod ? '#' : '3');
    } else if (event.keyCode == Common::Keys::Num4) {
        sysio->buffer(event.shiftMod ? '$' : '4');
    } else if (event.keyCode == Common::Keys::Num5) {
        sysio->buffer(event.shiftMod ? '%' : '5');
    } else if (event.keyCode == Common::Keys::Num6) {
        sysio->buffer(event.shiftMod ? '^' : '6');
    } else if (event.keyCode == Common::Keys::Num7) {
        sysio->buffer(event.shiftMod ? '&' : '7');
    } else if (event.keyCode == Common::Keys::Num8) {
        sysio->buffer(event.shiftMod ? '*' : '8');
    } else if (event.keyCode == Common::Keys::Num9) {
        sysio->buffer(event.shiftMod ? '(' : '9');
    } else if (event.keyCode == Common::Keys::Num0) {
        sysio->buffer(event.shiftMod ? ')' : '0');
    } else if (event.keyCode == Common::Keys::Backquote) {
        sysio->buffer(event.shiftMod ? '~' : '`');
    } else if (event.keyCode == Common::Keys::LBracket) {
        sysio->buffer(event.shiftMod ? '{' : '[');
    } else if (event.keyCode == Common::Keys::RBracket) {
        sysio->buffer(event.shiftMod ? '}' : ']');
    } else if (event.keyCode == Common::Keys::Semicolon) {
        sysio->buffer(event.shiftMod ? ':' : ';');
    } else if (event.keyCode == Common::Keys::Comma) {
        sysio->buffer(event.shiftMod ? '<' : ',');
    } else if (event.keyCode == Common::Keys::Period) {
        sysio->buffer(event.shiftMod ? '>' : '.');
    } else if (event.keyCode == Common::Keys::Quote) {
        sysio->buffer(event.shiftMod ? '"' : '\'');
    } else if (event.keyCode == Common::Keys::Slash) {
        sysio->buffer(event.shiftMod ? '?' : '/');
    } else if (event.keyCode == Common::Keys::Backslash) {
        sysio->buffer(event.shiftMod ? '|' : '\\');
    } else if (event.keyCode == Common::Keys::Equal) {
        sysio->buffer(event.shiftMod ? '+' : '=');
    } else if (event.keyCode == Common::Keys::Hyphen) {
        sysio->buffer(event.shiftMod ? '_' : '-');
    } else if (event.keyCode == Common::Keys::Enter) {
        sysio->buffer('\n');
    } else if (event.keyCode == Common::Keys::Tab) {
        sysio->buffer('\t');
    } else if (event.keyCode == Common::Keys::Space) {
        sysio->buffer(' ');
    } else if (event.keyCode == Common::Keys::Backspace) {
        sysio->buffer(8);
    } else if (event.keyCode == Common::Keys::F1) {
        state->changeState(1);
    } else if (event.keyCode == Common::Keys::F2) {
        state->changeState(2, std::make_any<std::shared_ptr<Emulator::Program>>(program));
    }
}

void EmulatorState::onKeyUp(State *state, const KeyPress &event) {
}
