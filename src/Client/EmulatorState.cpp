#include "Client/State.h"
#include "Client/EmulatorState.h"
#include "Common/Keys.h"
#include "Common/WaveForm.h"

#include <vector>
#include <iostream>

using namespace Client;

SystemIO::SystemIO() : cursor(0,0),currentPalette(1), background(0), foreground(255), fontSize(8) {
    palettes.resize(4);

    for (size_t i = 0; i < palettes[0].size(); i++) {
        palettes[0][i] = Common::Colour::Colour8(i);
    }

    palettes[1][0] = Common::Colour(0, 0, 0); //Black #000000
    palettes[1][1] = Common::Colour(128, 0, 0); //Maroon #800000
    palettes[1][2] = Common::Colour(0, 128, 0); //Green #008000
    palettes[1][3] = Common::Colour(128, 128, 0); //Olive #808000
    palettes[1][4] = Common::Colour(0, 0, 128); //Navy #000080
    palettes[1][5] = Common::Colour(128, 0, 128); //Purple #800080
    palettes[1][6] = Common::Colour(0, 128, 128); //Teal #008080
    palettes[1][7] = Common::Colour(192, 192, 192); //Silver #c0c0c0
    palettes[1][8] = Common::Colour(128, 128, 128); //Grey #808080
    palettes[1][9] = Common::Colour(255, 0, 0); //Red #ff0000
    palettes[1][10] = Common::Colour(0, 255, 0); //Lime #00ff00
    palettes[1][11] = Common::Colour(255, 255, 0); //Yellow #ffff00
    palettes[1][12] = Common::Colour(0, 0, 255); //Blue #0000ff
    palettes[1][13] = Common::Colour(255, 0, 255); //Fuchsia #ff00ff
    palettes[1][14] = Common::Colour(0, 255, 255); //Aqua #00ffff
    palettes[1][15] = Common::Colour(255, 255, 255); //White #ffffff
    palettes[1][16] = Common::Colour(0, 0, 0); //Grey0 #000000
    palettes[1][17] = Common::Colour(0, 0, 95); //NavyBlue #00005f
    palettes[1][18] = Common::Colour(0, 0, 135); //DarkBlue #000087
    palettes[1][19] = Common::Colour(0, 0, 175); //Blue3 #0000af
    palettes[1][20] = Common::Colour(0, 0, 215); //Blue3 #0000d7
    palettes[1][21] = Common::Colour(0, 0, 255); //Blue1 #0000ff
    palettes[1][22] = Common::Colour(0, 95, 0); //DarkGreen #005f00
    palettes[1][23] = Common::Colour(0, 95, 95); //DeepSkyBlue4 #005f5f
    palettes[1][24] = Common::Colour(0, 95, 135); //DeepSkyBlue4 #005f87
    palettes[1][25] = Common::Colour(0, 95, 175); //DeepSkyBlue4 #005faf
    palettes[1][26] = Common::Colour(0, 95, 215); //DodgerBlue3 #005fd7
    palettes[1][27] = Common::Colour(0, 95, 255); //DodgerBlue2 #005fff
    palettes[1][28] = Common::Colour(0, 135, 0); //Green4 #008700
    palettes[1][29] = Common::Colour(0, 135, 95); //SpringGreen4 #00875f
    palettes[1][30] = Common::Colour(0, 135, 135); //Turquoise4 #008787
    palettes[1][31] = Common::Colour(0, 135, 175); //DeepSkyBlue3 #0087af
    palettes[1][32] = Common::Colour(0, 135, 215); //DeepSkyBlue3 #0087d7
    palettes[1][33] = Common::Colour(0, 135, 255); //DodgerBlue1 #0087ff
    palettes[1][34] = Common::Colour(0, 175, 0); //Green3 #00af00
    palettes[1][35] = Common::Colour(0, 175, 95); //SpringGreen3 #00af5f
    palettes[1][36] = Common::Colour(0, 175, 135); //DarkCyan #00af87
    palettes[1][37] = Common::Colour(0, 175, 175); //LightSeaGreen #00afaf
    palettes[1][38] = Common::Colour(0, 175, 215); //DeepSkyBlue2 #00afd7
    palettes[1][39] = Common::Colour(0, 175, 255); //DeepSkyBlue1 #00afff
    palettes[1][40] = Common::Colour(0, 215, 0); //Green3 #00d700
    palettes[1][41] = Common::Colour(0, 215, 95); //SpringGreen3 #00d75f
    palettes[1][42] = Common::Colour(0, 215, 135); //SpringGreen2 #00d787
    palettes[1][43] = Common::Colour(0, 215, 175); //Cyan3 #00d7af
    palettes[1][44] = Common::Colour(0, 215, 215); //DarkTurquoise #00d7d7
    palettes[1][45] = Common::Colour(0, 215, 255); //Turquoise2 #00d7ff
    palettes[1][46] = Common::Colour(0, 255, 0); //Green1 #00ff00
    palettes[1][47] = Common::Colour(0, 255, 95); //SpringGreen2 #00ff5f
    palettes[1][48] = Common::Colour(0, 255, 135); //SpringGreen1 #00ff87
    palettes[1][49] = Common::Colour(0, 255, 175); //MediumSpringGreen #00ffaf
    palettes[1][50] = Common::Colour(0, 255, 215); //Cyan2 #00ffd7
    palettes[1][51] = Common::Colour(0, 255, 255); //Cyan1 #00ffff
    palettes[1][52] = Common::Colour(95, 0, 0); //DarkRed #5f0000
    palettes[1][53] = Common::Colour(95, 0, 95); //DeepPink4 #5f005f
    palettes[1][54] = Common::Colour(95, 0, 135); //Purple4 #5f0087
    palettes[1][55] = Common::Colour(95, 0, 175); //Purple4 #5f00af
    palettes[1][56] = Common::Colour(95, 0, 215); //Purple3 #5f00d7
    palettes[1][57] = Common::Colour(95, 0, 255); //BlueViolet #5f00ff
    palettes[1][58] = Common::Colour(95, 95, 0); //Orange4 #5f5f00
    palettes[1][59] = Common::Colour(95, 95, 95); //Grey37 #5f5f5f
    palettes[1][60] = Common::Colour(95, 95, 135); //MediumPurple4 #5f5f87
    palettes[1][61] = Common::Colour(95, 95, 175); //SlateBlue3 #5f5faf
    palettes[1][62] = Common::Colour(95, 95, 215); //SlateBlue3 #5f5fd7
    palettes[1][63] = Common::Colour(95, 95, 255); //RoyalBlue1 #5f5fff
    palettes[1][64] = Common::Colour(95, 135, 0); //Chartreuse4 #5f8700
    palettes[1][65] = Common::Colour(95, 135, 95); //DarkSeaGreen4 #5f875f
    palettes[1][66] = Common::Colour(95, 135, 135); //PaleTurquoise4 #5f8787
    palettes[1][67] = Common::Colour(95, 135, 175); //SteelBlue #5f87af
    palettes[1][68] = Common::Colour(95, 135, 215); //SteelBlue3 #5f87d7
    palettes[1][69] = Common::Colour(95, 135, 255); //CornflowerBlue #5f87ff
    palettes[1][70] = Common::Colour(95, 175, 0); //Chartreuse3 #5faf00
    palettes[1][71] = Common::Colour(95, 175, 95); //DarkSeaGreen4 #5faf5f
    palettes[1][72] = Common::Colour(95, 175, 135); //CadetBlue #5faf87
    palettes[1][73] = Common::Colour(95, 175, 175); //CadetBlue #5fafaf
    palettes[1][74] = Common::Colour(95, 175, 215); //SkyBlue3 #5fafd7
    palettes[1][75] = Common::Colour(95, 175, 255); //SteelBlue1 #5fafff
    palettes[1][76] = Common::Colour(95, 215, 0); //Chartreuse3 #5fd700
    palettes[1][77] = Common::Colour(95, 215, 95); //PaleGreen3 #5fd75f
    palettes[1][78] = Common::Colour(95, 215, 135); //SeaGreen3 #5fd787
    palettes[1][79] = Common::Colour(95, 215, 175); //Aquamarine3 #5fd7af
    palettes[1][80] = Common::Colour(95, 215, 215); //MediumTurquoise #5fd7d7
    palettes[1][81] = Common::Colour(95, 215, 255); //SteelBlue1 #5fd7ff
    palettes[1][82] = Common::Colour(95, 255, 0); //Chartreuse2 #5fff00
    palettes[1][83] = Common::Colour(95, 255, 95); //SeaGreen2 #5fff5f
    palettes[1][84] = Common::Colour(95, 255, 135); //SeaGreen1 #5fff87
    palettes[1][85] = Common::Colour(95, 255, 175); //SeaGreen1 #5fffaf
    palettes[1][86] = Common::Colour(95, 255, 215); //Aquamarine1 #5fffd7
    palettes[1][87] = Common::Colour(95, 255, 255); //DarkSlateGray2 #5fffff
    palettes[1][88] = Common::Colour(135, 0, 0); //DarkRed #870000
    palettes[1][89] = Common::Colour(135, 0, 95); //DeepPink4 #87005f
    palettes[1][90] = Common::Colour(135, 0, 135); //DarkMagenta #870087
    palettes[1][91] = Common::Colour(135, 0, 175); //DarkMagenta #8700af
    palettes[1][92] = Common::Colour(135, 0, 215); //DarkViolet #8700d7
    palettes[1][93] = Common::Colour(135, 0, 255); //Purple #8700ff
    palettes[1][94] = Common::Colour(135, 95, 0); //Orange4 #875f00
    palettes[1][95] = Common::Colour(135, 95, 95); //LightPink4 #875f5f
    palettes[1][96] = Common::Colour(135, 95, 135); //Plum4 #875f87
    palettes[1][97] = Common::Colour(135, 95, 175); //MediumPurple3 #875faf
    palettes[1][98] = Common::Colour(135, 95, 215); //MediumPurple3 #875fd7
    palettes[1][99] = Common::Colour(135, 95, 255); //SlateBlue1 #875fff
    palettes[1][100] = Common::Colour(135, 135, 0); //Yellow4 #878700
    palettes[1][101] = Common::Colour(135, 135, 95); //Wheat4 #87875f
    palettes[1][102] = Common::Colour(135, 135, 135); //Grey53 #878787
    palettes[1][103] = Common::Colour(135, 135, 175); //LightSlateGrey #8787af
    palettes[1][104] = Common::Colour(135, 135, 215); //MediumPurple #8787d7
    palettes[1][105] = Common::Colour(135, 135, 255); //LightSlateBlue #8787ff
    palettes[1][106] = Common::Colour(135, 175, 0); //Yellow4 #87af00
    palettes[1][107] = Common::Colour(135, 175, 95); //DarkOliveGreen3 #87af5f
    palettes[1][108] = Common::Colour(135, 175, 135); //DarkSeaGreen #87af87
    palettes[1][109] = Common::Colour(135, 175, 175); //LightSkyBlue3 #87afaf
    palettes[1][110] = Common::Colour(135, 175, 215); //LightSkyBlue3 #87afd7
    palettes[1][111] = Common::Colour(135, 175, 255); //SkyBlue2 #87afff
    palettes[1][112] = Common::Colour(135, 215, 0); //Chartreuse2 #87d700
    palettes[1][113] = Common::Colour(135, 215, 95); //DarkOliveGreen3 #87d75f
    palettes[1][114] = Common::Colour(135, 215, 135); //PaleGreen3 #87d787
    palettes[1][115] = Common::Colour(135, 215, 175); //DarkSeaGreen3 #87d7af
    palettes[1][116] = Common::Colour(135, 215, 215); //DarkSlateGray3 #87d7d7
    palettes[1][117] = Common::Colour(135, 215, 255); //SkyBlue1 #87d7ff
    palettes[1][118] = Common::Colour(135, 255, 0); //Chartreuse1 #87ff00
    palettes[1][119] = Common::Colour(135, 255, 95); //LightGreen #87ff5f
    palettes[1][120] = Common::Colour(135, 255, 135); //LightGreen #87ff87
    palettes[1][121] = Common::Colour(135, 255, 175); //PaleGreen1 #87ffaf
    palettes[1][122] = Common::Colour(135, 255, 215); //Aquamarine1 #87ffd7
    palettes[1][123] = Common::Colour(135, 255, 255); //DarkSlateGray1 #87ffff
    palettes[1][124] = Common::Colour(175, 0, 0); //Red3 #af0000
    palettes[1][125] = Common::Colour(175, 0, 95); //DeepPink4 #af005f
    palettes[1][126] = Common::Colour(175, 0, 135); //MediumVioletRed #af0087
    palettes[1][127] = Common::Colour(175, 0, 175); //Magenta3 #af00af
    palettes[1][128] = Common::Colour(175, 0, 215); //DarkViolet #af00d7
    palettes[1][129] = Common::Colour(175, 0, 255); //Purple #af00ff
    palettes[1][130] = Common::Colour(175, 95, 0); //DarkOrange3 #af5f00
    palettes[1][131] = Common::Colour(175, 95, 95); //IndianRed #af5f5f
    palettes[1][132] = Common::Colour(175, 95, 135); //HotPink3 #af5f87
    palettes[1][133] = Common::Colour(175, 95, 175); //MediumOrchid3 #af5faf
    palettes[1][134] = Common::Colour(175, 95, 215); //MediumOrchid #af5fd7
    palettes[1][135] = Common::Colour(175, 95, 255); //MediumPurple2 #af5fff
    palettes[1][136] = Common::Colour(175, 135, 0); //DarkGoldenrod #af8700
    palettes[1][137] = Common::Colour(175, 135, 95); //LightSalmon3 #af875f
    palettes[1][138] = Common::Colour(175, 135, 135); //RosyBrown #af8787
    palettes[1][139] = Common::Colour(175, 135, 175); //Grey63 #af87af
    palettes[1][140] = Common::Colour(175, 135, 215); //MediumPurple2 #af87d7
    palettes[1][141] = Common::Colour(175, 135, 255); //MediumPurple1 #af87ff
    palettes[1][142] = Common::Colour(175, 175, 0); //Gold3 #afaf00
    palettes[1][143] = Common::Colour(175, 175, 95); //DarkKhaki #afaf5f
    palettes[1][144] = Common::Colour(175, 175, 135); //NavajoWhite3 #afaf87
    palettes[1][145] = Common::Colour(175, 175, 175); //Grey69 #afafaf
    palettes[1][146] = Common::Colour(175, 175, 215); //LightSteelBlue3 #afafd7
    palettes[1][147] = Common::Colour(175, 175, 255); //LightSteelBlue #afafff
    palettes[1][148] = Common::Colour(175, 215, 0); //Yellow3 #afd700
    palettes[1][149] = Common::Colour(175, 215, 95); //DarkOliveGreen3 #afd75f
    palettes[1][150] = Common::Colour(175, 215, 135); //DarkSeaGreen3 #afd787
    palettes[1][151] = Common::Colour(175, 215, 175); //DarkSeaGreen2 #afd7af
    palettes[1][152] = Common::Colour(175, 215, 215); //LightCyan3 #afd7d7
    palettes[1][153] = Common::Colour(175, 215, 255); //LightSkyBlue1 #afd7ff
    palettes[1][154] = Common::Colour(175, 255, 0); //GreenYellow #afff00
    palettes[1][155] = Common::Colour(175, 255, 95); //DarkOliveGreen2 #afff5f
    palettes[1][156] = Common::Colour(175, 255, 135); //PaleGreen1 #afff87
    palettes[1][157] = Common::Colour(175, 255, 175); //DarkSeaGreen2 #afffaf
    palettes[1][158] = Common::Colour(175, 255, 215); //DarkSeaGreen1 #afffd7
    palettes[1][159] = Common::Colour(175, 255, 255); //PaleTurquoise1 #afffff
    palettes[1][160] = Common::Colour(215, 0, 0); //Red3 #d70000
    palettes[1][161] = Common::Colour(215, 0, 95); //DeepPink3 #d7005f
    palettes[1][162] = Common::Colour(215, 0, 135); //DeepPink3 #d70087
    palettes[1][163] = Common::Colour(215, 0, 175); //Magenta3 #d700af
    palettes[1][164] = Common::Colour(215, 0, 215); //Magenta3 #d700d7
    palettes[1][165] = Common::Colour(215, 0, 255); //Magenta2 #d700ff
    palettes[1][166] = Common::Colour(215, 95, 0); //DarkOrange3 #d75f00
    palettes[1][167] = Common::Colour(215, 95, 95); //IndianRed #d75f5f
    palettes[1][168] = Common::Colour(215, 95, 135); //HotPink3 #d75f87
    palettes[1][169] = Common::Colour(215, 95, 175); //HotPink2 #d75faf
    palettes[1][170] = Common::Colour(215, 95, 215); //Orchid #d75fd7
    palettes[1][171] = Common::Colour(215, 95, 255); //MediumOrchid1 #d75fff
    palettes[1][172] = Common::Colour(215, 135, 0); //Orange3 #d78700
    palettes[1][173] = Common::Colour(215, 135, 95); //LightSalmon3 #d7875f
    palettes[1][174] = Common::Colour(215, 135, 135); //LightPink3 #d78787
    palettes[1][175] = Common::Colour(215, 135, 175); //Pink3 #d787af
    palettes[1][176] = Common::Colour(215, 135, 215); //Plum3 #d787d7
    palettes[1][177] = Common::Colour(215, 135, 255); //Violet #d787ff
    palettes[1][178] = Common::Colour(215, 175, 0); //Gold3 #d7af00
    palettes[1][179] = Common::Colour(215, 175, 95); //LightGoldenrod3 #d7af5f
    palettes[1][180] = Common::Colour(215, 175, 135); //Tan #d7af87
    palettes[1][181] = Common::Colour(215, 175, 175); //MistyRose3 #d7afaf
    palettes[1][182] = Common::Colour(215, 175, 215); //Thistle3 #d7afd7
    palettes[1][183] = Common::Colour(215, 175, 255); //Plum2 #d7afff
    palettes[1][184] = Common::Colour(215, 215, 0); //Yellow3 #d7d700
    palettes[1][185] = Common::Colour(215, 215, 95); //Khaki3 #d7d75f
    palettes[1][186] = Common::Colour(215, 215, 135); //LightGoldenrod2 #d7d787
    palettes[1][187] = Common::Colour(215, 215, 175); //LightYellow3 #d7d7af
    palettes[1][188] = Common::Colour(215, 215, 215); //Grey84 #d7d7d7
    palettes[1][189] = Common::Colour(215, 215, 255); //LightSteelBlue1 #d7d7ff
    palettes[1][190] = Common::Colour(215, 255, 0); //Yellow2 #d7ff00
    palettes[1][191] = Common::Colour(215, 255, 95); //DarkOliveGreen1 #d7ff5f
    palettes[1][192] = Common::Colour(215, 255, 135); //DarkOliveGreen1 #d7ff87
    palettes[1][193] = Common::Colour(215, 255, 175); //DarkSeaGreen1 #d7ffaf
    palettes[1][194] = Common::Colour(215, 255, 215); //Honeydew2 #d7ffd7
    palettes[1][195] = Common::Colour(215, 255, 255); //LightCyan1 #d7ffff
    palettes[1][196] = Common::Colour(255, 0, 0); //Red1 #ff0000
    palettes[1][197] = Common::Colour(255, 0, 95); //DeepPink2 #ff005f
    palettes[1][198] = Common::Colour(255, 0, 135); //DeepPink1 #ff0087
    palettes[1][199] = Common::Colour(255, 0, 175); //DeepPink1 #ff00af
    palettes[1][200] = Common::Colour(255, 0, 215); //Magenta2 #ff00d7
    palettes[1][201] = Common::Colour(255, 0, 255); //Magenta1 #ff00ff
    palettes[1][202] = Common::Colour(255, 95, 0); //OrangeRed1 #ff5f00
    palettes[1][203] = Common::Colour(255, 95, 95); //IndianRed1 #ff5f5f
    palettes[1][204] = Common::Colour(255, 95, 135); //IndianRed1 #ff5f87
    palettes[1][205] = Common::Colour(255, 95, 175); //HotPink #ff5faf
    palettes[1][206] = Common::Colour(255, 95, 215); //HotPink #ff5fd7
    palettes[1][207] = Common::Colour(255, 95, 255); //MediumOrchid1 #ff5fff
    palettes[1][208] = Common::Colour(255, 135, 0); //DarkOrange #ff8700
    palettes[1][209] = Common::Colour(255, 135, 95); //Salmon1 #ff875f
    palettes[1][210] = Common::Colour(255, 135, 135); //LightCoral #ff8787
    palettes[1][211] = Common::Colour(255, 135, 175); //PaleVioletRed1 #ff87af
    palettes[1][212] = Common::Colour(255, 135, 215); //Orchid2 #ff87d7
    palettes[1][213] = Common::Colour(255, 135, 255); //Orchid1 #ff87ff
    palettes[1][214] = Common::Colour(255, 175, 0); //Orange1 #ffaf00
    palettes[1][215] = Common::Colour(255, 175, 95); //SandyBrown #ffaf5f
    palettes[1][216] = Common::Colour(255, 175, 135); //LightSalmon1 #ffaf87
    palettes[1][217] = Common::Colour(255, 175, 175); //LightPink1 #ffafaf
    palettes[1][218] = Common::Colour(255, 175, 215); //Pink1 #ffafd7
    palettes[1][219] = Common::Colour(255, 175, 255); //Plum1 #ffafff
    palettes[1][220] = Common::Colour(255, 215, 0); //Gold1 #ffd700
    palettes[1][221] = Common::Colour(255, 215, 95); //LightGoldenrod2 #ffd75f
    palettes[1][222] = Common::Colour(255, 215, 135); //LightGoldenrod2 #ffd787
    palettes[1][223] = Common::Colour(255, 215, 175); //NavajoWhite1 #ffd7af
    palettes[1][224] = Common::Colour(255, 215, 215); //MistyRose1 #ffd7d7
    palettes[1][225] = Common::Colour(255, 215, 255); //Thistle1 #ffd7ff
    palettes[1][226] = Common::Colour(255, 255, 0); //Yellow1 #ffff00
    palettes[1][227] = Common::Colour(255, 255, 95); //LightGoldenrod1 #ffff5f
    palettes[1][228] = Common::Colour(255, 255, 135); //Khaki1 #ffff87
    palettes[1][229] = Common::Colour(255, 255, 175); //Wheat1 #ffffaf
    palettes[1][230] = Common::Colour(255, 255, 215); //Cornsilk1 #ffffd7
    palettes[1][231] = Common::Colour(255, 255, 255); //Grey100 #ffffff
    palettes[1][232] = Common::Colour(8, 8, 8); //Grey3 #080808
    palettes[1][233] = Common::Colour(18, 18, 18); //Grey7 #121212
    palettes[1][234] = Common::Colour(28, 28, 28); //Grey11 #1c1c1c
    palettes[1][235] = Common::Colour(38, 38, 38); //Grey15 #262626
    palettes[1][236] = Common::Colour(48, 48, 48); //Grey19 #303030
    palettes[1][237] = Common::Colour(58, 58, 58); //Grey23 #3a3a3a
    palettes[1][238] = Common::Colour(68, 68, 68); //Grey27 #444444
    palettes[1][239] = Common::Colour(78, 78, 78); //Grey30 #4e4e4e
    palettes[1][240] = Common::Colour(88, 88, 88); //Grey35 #585858
    palettes[1][241] = Common::Colour(98, 98, 98); //Grey39 #626262
    palettes[1][242] = Common::Colour(108, 108, 108); //Grey42 #6c6c6c
    palettes[1][243] = Common::Colour(118, 118, 118); //Grey46 #767676
    palettes[1][244] = Common::Colour(128, 128, 128); //Grey50 #808080
    palettes[1][245] = Common::Colour(138, 138, 138); //Grey54 #8a8a8a
    palettes[1][246] = Common::Colour(148, 148, 148); //Grey58 #949494
    palettes[1][247] = Common::Colour(158, 158, 158); //Grey62 #9e9e9e
    palettes[1][248] = Common::Colour(168, 168, 168); //Grey66 #a8a8a8
    palettes[1][249] = Common::Colour(178, 178, 178); //Grey70 #b2b2b2
    palettes[1][250] = Common::Colour(188, 188, 188); //Grey74 #bcbcbc
    palettes[1][251] = Common::Colour(198, 198, 198); //Grey78 #c6c6c6
    palettes[1][252] = Common::Colour(208, 208, 208); //Grey82 #d0d0d0
    palettes[1][253] = Common::Colour(218, 218, 218); //Grey85 #dadada
    palettes[1][254] = Common::Colour(228, 228, 228); //Grey89 #e4e4e4
    palettes[1][255] = Common::Colour(238, 238, 238); //Grey93 #eeeeee

    size_t index = 0;

    // CGA color palette
    palettes[2][index++] = Common::Colour(0, 0, 0);          // 0 - black
    palettes[2][index++] = Common::Colour(0, 0, 170);        // 1 - blue
    palettes[2][index++] = Common::Colour(0, 170, 0);        // 2 - green 
    palettes[2][index++] = Common::Colour(0, 170, 170);      // 3 - cyan
    palettes[2][index++] = Common::Colour(170, 0, 0);        // 4 - red 
    palettes[2][index++] = Common::Colour(170, 0, 170);      // 5 - magenta
    palettes[2][index++] = Common::Colour(170, 85, 170);     // 6 - brown
    palettes[2][index++] = Common::Colour(170, 170, 170);    // 7 - gray
    palettes[2][index++] = Common::Colour(85, 85, 85);       // 8 - lighter black
    palettes[2][index++] = Common::Colour(85, 85, 255);      // 9 - lighter blue
    palettes[2][index++] = Common::Colour(85, 255, 85);      // A - lighter green 
    palettes[2][index++] = Common::Colour(85, 255, 255);     // B - lighter cyan
    palettes[2][index++] = Common::Colour(255, 85, 85);      // C - lighter red 
    palettes[2][index++] = Common::Colour(255, 85, 255);     // D - lighter magenta
    palettes[2][index++] = Common::Colour(255, 255, 85);     // E - lighter brown
    palettes[2][index++] = Common::Colour(255, 255, 255);    // F - lighter gray

    // greyscale
    {
        uint8_t grey = 0x00;
        for (int i = 0; i < 16; i++) {
            palettes[2][index++] = Common::Colour(grey, grey, grey);
            grey += 0x10;
        }
    }

    // value array
    std::array<double,3> satLum = {1.0, 0.5, 0.25};

    // general rainbow for various luminances
    for (double lum : satLum) {
        for (double sat : satLum) {
            for (int i = 240; i < 240 + 360; i += 15) {
                palettes[2][index++] = Common::Colour::HSV(i, sat, lum);
            }
        }
    }

    // fill the rest of the palette with black
    while (index < palettes[2].size())
        palettes[2][index++] = Common::Colour(0, 0, 0);

    for (size_t i = 0; i < palettes[1].size(); i++) {
        palettes[3][i] = !palettes[1][i];
    }

    cls();

    screenbuffer[cursor.Y()][cursor.X()] = (char)255;

    nextKeyId = 1;
}

void SystemIO::cls() {
    screen.fill(background);
    cursor = Point(0,0);
    screenbuffer.clear();
    screenbuffer.resize(lines);
    for (size_t i=0; i < screenbuffer.size(); i++) {
        screenbuffer[i].fill(' ');
        screenbuffer[i][screenbuffer.size()] = 0;
    }
}

void SystemIO::setpixel(uint16_t x, uint16_t y, uint8_t pixel) {
    if (x < Width && y < Height)
        screen[y*Width + x] = pixel;
}

uint8_t SystemIO::getpixel(uint16_t x, uint16_t y) {
    if (x < Width && y < Height)
        return screen[y*Width + x];
    return 0;
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
        screenbuffer[cursor.Y()][cursor.X()] = (char)255;
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

bool SystemIO::keyset(const uint8_t c) {
    if (inputBuffer.size() && (uint8_t)inputBuffer.front() == c) {
        inputBuffer.pop();
        return true;
    }

    return keysPressed[c] != 0;
}

void SystemIO::mousestate(int16_t &x, int16_t &y, uint16_t &buttonState) {
    x = (int16_t)mouse.X();
    y = (int16_t)mouse.Y();
    buttonState = mouseButtons;
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
    uint8_t *ptr = screen.data();
    ptr += y*Width + x;

    std::memcpy(ptr, buffer.data(), buffer.size());
}

void SystemIO::sound(uint8_t voice, float frequency, uint16_t duration) {
    auto voiceConfig = voices[voice];

    soundBuffer.push(SoundBufferObject(voice, frequency, duration, voiceConfig));
}

void SystemIO::voice(uint8_t voice, uint8_t waveForm, uint8_t volume, uint8_t attack, uint8_t decay, uint8_t sustain, uint8_t release) {
    voices[voice] = VoiceConfig(waveForm, volume, attack, decay, sustain, release);
}

/*
std::queue<char> SystemIO::buffer() const {
    std::vector<char> keys;

    for (size_t i = 0; i < keysPressed.size(); i++) {
        uint64_t keyId = keysPressed[i];

        if (keyId == 0)
            continue;

        if (keys.size() == 0) {
            keys.push_back((char)i);
        } else if (keyId >= keysPressed[keys[keys.size()-1]]) {
            keys.push_back((char)i);
        } else {
            for (size_t k = 0; k < keys.size(); k++) {
                uint64_t existingId = keysPressed[keys[k]];
                if (keyId < existingId) {
                    keys.insert(keys.begin() + k, (char)i);
                    break;
                } 
            }
        }
    }

    if (keys.size())
        std::cerr << std::string(keys.data()) << std::endl;

    std::queue<char, std::deque<char>> buffer(std::deque<char>(keys.begin(), keys.end()));

    return buffer;
}
*/

void SystemIO::keydown(char key) {
    inputBuffer.push(key);
    keysPressed[key] = nextKeyId++;
}

void SystemIO::keyup(char key) {
    keysPressed[key] = 0;
}

void SystemIO::mousemove(const Point &move) {
    mouse = move;
}

void SystemIO::mousedown(const MouseClick &click) {
    if (click.leftPressed)
        mouseButtons |= LeftPressed;
    if (click.middlePressed)
        mouseButtons |= MiddlePressed;
    if (click.rightPressed)
        mouseButtons |= RightPressed;
    if (click.x1Pressed)
        mouseButtons |= X1Pressed;
    if (click.x2Pressed)
        mouseButtons |= X2Pressed;
}

void SystemIO::mouseup(const MouseClick &click) {
    if (click.leftPressed)
        mouseButtons &= ~LeftPressed;
    if (click.middlePressed)
        mouseButtons &= ~MiddlePressed;
    if (click.rightPressed)
        mouseButtons &= ~RightPressed;
    if (click.x1Pressed)
        mouseButtons &= ~X1Pressed;
    if (click.x2Pressed)
        mouseButtons &= ~X2Pressed;
}


void EmulatorState::onRender(State *state, const uint32_t time) {
    static std::array<uint32_t, SystemIO::Width * SystemIO::Height> buffer;
    auto screen = sysio->getScreen();
    auto palette = sysio->getCurrentPalette();

    std::transform(screen.begin(), screen.end(), buffer.begin(),
        [palette](uint8_t pixel) {
            return palette[pixel].RGBA();
        }
    );

    state->getRenderer()->drawBuffer(buffer.data(), SystemIO::Width, SystemIO::Height);

    uint16_t lineoffset = 0;
    for (auto line : sysio->getScreenBuffer()) {
        state->getRenderer()->drawString(0, lineoffset*sysio->FontSize(), sysio->FontSize(), sysio->FontSize(), std::string(line.data()), sysio->getForeground().RGBA());
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

    sysio->setTime(time);

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
                    done = false;
                } catch (const std::invalid_argument &ia) {
                    sysio->puts(ia.what() + std::string("\n"));
                } catch (const std::domain_error &de) {
                    sysio->puts(de.what() + std::string("\n"));
                }
            } else if (cmd == "CLS") {
                sysio->cls();
            } else if (cmd == "CLEAR") {
                basic.clear();
            } else {
                auto basicline = parseLine(input);
                if (basicline.first) {
                    basic[basicline.first] = basicline.second;
                } else {
                    //sysio->puts("? Unknown command\n");
                    auto run = program->add(Emulator::OpCode::NOP);
                    std::map<uint32_t, std::vector<Emulator::BasicToken>> cmd = {{basicline.first, basicline.second}};
                    try {
                        compile(cmd, *program);
                        vm->Jump(run);
                        done = false;
                    } catch (const std::invalid_argument &ia) {
                        sysio->puts(ia.what() + std::string("\n"));
                    } catch (const std::domain_error &de) {
                        sysio->puts(de.what() + std::string("\n"));
                    }
                    sysio->puts("OK\n");
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
    Point move;
    Point real(event.x, event.y);
    if (state->getRenderer()->translatePoint(move, real)) {
        sysio->mousemove(move);
    }
}

void EmulatorState::onMouseButtonPress(State *state, const MouseClick &event) {
    sysio->mousedown(event);
}

void EmulatorState::onMouseButtonRelease(State *state, const MouseClick &event) {
    sysio->mouseup(event);
}

void EmulatorState::onKeyDown(State *state, const KeyPress &event) {
    if (event.keyCode == Common::Keys::A) {
        sysio->keydown(event.shiftMod ? 'A' : 'a');
    } else if (event.keyCode == Common::Keys::B) {
        sysio->keydown(event.shiftMod ? 'B' : 'b');
    } else if (event.keyCode == Common::Keys::C) {
        sysio->keydown(event.shiftMod ? 'C' : 'c');
    } else if (event.keyCode == Common::Keys::D) {
        sysio->keydown(event.shiftMod ? 'D' : 'd');
    } else if (event.keyCode == Common::Keys::E) {
        sysio->keydown(event.shiftMod ? 'E' : 'e');
    } else if (event.keyCode == Common::Keys::F) {
        sysio->keydown(event.shiftMod ? 'F' : 'f');
    } else if (event.keyCode == Common::Keys::G) {
        sysio->keydown(event.shiftMod ? 'G' : 'g');
    } else if (event.keyCode == Common::Keys::H) {
        sysio->keydown(event.shiftMod ? 'H' : 'h');
    } else if (event.keyCode == Common::Keys::I) {
        sysio->keydown(event.shiftMod ? 'I' : 'i');
    } else if (event.keyCode == Common::Keys::J) {
        sysio->keydown(event.shiftMod ? 'J' : 'j');
    } else if (event.keyCode == Common::Keys::K) {
        sysio->keydown(event.shiftMod ? 'K' : 'k');
    } else if (event.keyCode == Common::Keys::L) {
        sysio->keydown(event.shiftMod ? 'L' : 'l');
    } else if (event.keyCode == Common::Keys::M) {
        sysio->keydown(event.shiftMod ? 'M' : 'm');
    } else if (event.keyCode == Common::Keys::N) {
        sysio->keydown(event.shiftMod ? 'N' : 'n');
    } else if (event.keyCode == Common::Keys::O) {
        sysio->keydown(event.shiftMod ? 'O' : 'o');
    } else if (event.keyCode == Common::Keys::P) {
        sysio->keydown(event.shiftMod ? 'P' : 'p');
    } else if (event.keyCode == Common::Keys::Q) {
        sysio->keydown(event.shiftMod ? 'Q' : 'q');
    } else if (event.keyCode == Common::Keys::R) {
        sysio->keydown(event.shiftMod ? 'R' : 'r');
    } else if (event.keyCode == Common::Keys::S) {
        sysio->keydown(event.shiftMod ? 'S' : 's');
    } else if (event.keyCode == Common::Keys::T) {
        sysio->keydown(event.shiftMod ? 'T' : 't');
    } else if (event.keyCode == Common::Keys::U) {
        sysio->keydown(event.shiftMod ? 'U' : 'u');
    } else if (event.keyCode == Common::Keys::V) {
        sysio->keydown(event.shiftMod ? 'V' : 'v');
    } else if (event.keyCode == Common::Keys::W) {
        sysio->keydown(event.shiftMod ? 'W' : 'w');
    } else if (event.keyCode == Common::Keys::X) {
        sysio->keydown(event.shiftMod ? 'X' : 'x');
    } else if (event.keyCode == Common::Keys::Y) {
        sysio->keydown(event.shiftMod ? 'Y' : 'y');
    } else if (event.keyCode == Common::Keys::Z) {
        sysio->keydown(event.shiftMod ? 'Z' : 'z');
    } else if (event.keyCode == Common::Keys::Num1) {
        sysio->keydown(event.shiftMod ? '!' : '1');
    } else if (event.keyCode == Common::Keys::Num2) {
        sysio->keydown(event.shiftMod ? '@' : '2');
    } else if (event.keyCode == Common::Keys::Num3) {
        sysio->keydown(event.shiftMod ? '#' : '3');
    } else if (event.keyCode == Common::Keys::Num4) {
        sysio->keydown(event.shiftMod ? '$' : '4');
    } else if (event.keyCode == Common::Keys::Num5) {
        sysio->keydown(event.shiftMod ? '%' : '5');
    } else if (event.keyCode == Common::Keys::Num6) {
        sysio->keydown(event.shiftMod ? '^' : '6');
    } else if (event.keyCode == Common::Keys::Num7) {
        sysio->keydown(event.shiftMod ? '&' : '7');
    } else if (event.keyCode == Common::Keys::Num8) {
        sysio->keydown(event.shiftMod ? '*' : '8');
    } else if (event.keyCode == Common::Keys::Num9) {
        sysio->keydown(event.shiftMod ? '(' : '9');
    } else if (event.keyCode == Common::Keys::Num0) {
        sysio->keydown(event.shiftMod ? ')' : '0');
    } else if (event.keyCode == Common::Keys::Backquote) {
        sysio->keydown(event.shiftMod ? '~' : '`');
    } else if (event.keyCode == Common::Keys::LBracket) {
        sysio->keydown(event.shiftMod ? '{' : '[');
    } else if (event.keyCode == Common::Keys::RBracket) {
        sysio->keydown(event.shiftMod ? '}' : ']');
    } else if (event.keyCode == Common::Keys::Semicolon) {
        sysio->keydown(event.shiftMod ? ':' : ';');
    } else if (event.keyCode == Common::Keys::Comma) {
        sysio->keydown(event.shiftMod ? '<' : ',');
    } else if (event.keyCode == Common::Keys::Period) {
        sysio->keydown(event.shiftMod ? '>' : '.');
    } else if (event.keyCode == Common::Keys::Quote) {
        sysio->keydown(event.shiftMod ? '"' : '\'');
    } else if (event.keyCode == Common::Keys::Slash) {
        sysio->keydown(event.shiftMod ? '?' : '/');
    } else if (event.keyCode == Common::Keys::Backslash) {
        sysio->keydown(event.shiftMod ? '|' : '\\');
    } else if (event.keyCode == Common::Keys::Equal) {
        sysio->keydown(event.shiftMod ? '+' : '=');
    } else if (event.keyCode == Common::Keys::Hyphen) {
        sysio->keydown(event.shiftMod ? '_' : '-');
    } else if (event.keyCode == Common::Keys::Enter) {
        sysio->keydown('\n');
    } else if (event.keyCode == Common::Keys::Tab) {
        sysio->keydown('\t');
    } else if (event.keyCode == Common::Keys::Space) {
        sysio->keydown(' ');
    } else if (event.keyCode == Common::Keys::Backspace) {
        sysio->keydown(8);
    } else if (event.keyCode == Common::Keys::F1) {
        state->changeState(1);
    } else if (event.keyCode == Common::Keys::F2) {
        state->changeState(2, std::make_any<std::shared_ptr<Emulator::Program>>(program));
    }
}

void EmulatorState::onKeyUp(State *state, const KeyPress &event) {
    if (event.keyCode == Common::Keys::A) {
        sysio->keyup(event.shiftMod ? 'A' : 'a');
    } else if (event.keyCode == Common::Keys::B) {
        sysio->keyup(event.shiftMod ? 'B' : 'b');
    } else if (event.keyCode == Common::Keys::C) {
        sysio->keyup(event.shiftMod ? 'C' : 'c');
    } else if (event.keyCode == Common::Keys::D) {
        sysio->keyup(event.shiftMod ? 'D' : 'd');
    } else if (event.keyCode == Common::Keys::E) {
        sysio->keyup(event.shiftMod ? 'E' : 'e');
    } else if (event.keyCode == Common::Keys::F) {
        sysio->keyup(event.shiftMod ? 'F' : 'f');
    } else if (event.keyCode == Common::Keys::G) {
        sysio->keyup(event.shiftMod ? 'G' : 'g');
    } else if (event.keyCode == Common::Keys::H) {
        sysio->keyup(event.shiftMod ? 'H' : 'h');
    } else if (event.keyCode == Common::Keys::I) {
        sysio->keyup(event.shiftMod ? 'I' : 'i');
    } else if (event.keyCode == Common::Keys::J) {
        sysio->keyup(event.shiftMod ? 'J' : 'j');
    } else if (event.keyCode == Common::Keys::K) {
        sysio->keyup(event.shiftMod ? 'K' : 'k');
    } else if (event.keyCode == Common::Keys::L) {
        sysio->keyup(event.shiftMod ? 'L' : 'l');
    } else if (event.keyCode == Common::Keys::M) {
        sysio->keyup(event.shiftMod ? 'M' : 'm');
    } else if (event.keyCode == Common::Keys::N) {
        sysio->keyup(event.shiftMod ? 'N' : 'n');
    } else if (event.keyCode == Common::Keys::O) {
        sysio->keyup(event.shiftMod ? 'O' : 'o');
    } else if (event.keyCode == Common::Keys::P) {
        sysio->keyup(event.shiftMod ? 'P' : 'p');
    } else if (event.keyCode == Common::Keys::Q) {
        sysio->keyup(event.shiftMod ? 'Q' : 'q');
    } else if (event.keyCode == Common::Keys::R) {
        sysio->keyup(event.shiftMod ? 'R' : 'r');
    } else if (event.keyCode == Common::Keys::S) {
        sysio->keyup(event.shiftMod ? 'S' : 's');
    } else if (event.keyCode == Common::Keys::T) {
        sysio->keyup(event.shiftMod ? 'T' : 't');
    } else if (event.keyCode == Common::Keys::U) {
        sysio->keyup(event.shiftMod ? 'U' : 'u');
    } else if (event.keyCode == Common::Keys::V) {
        sysio->keyup(event.shiftMod ? 'V' : 'v');
    } else if (event.keyCode == Common::Keys::W) {
        sysio->keyup(event.shiftMod ? 'W' : 'w');
    } else if (event.keyCode == Common::Keys::X) {
        sysio->keyup(event.shiftMod ? 'X' : 'x');
    } else if (event.keyCode == Common::Keys::Y) {
        sysio->keyup(event.shiftMod ? 'Y' : 'y');
    } else if (event.keyCode == Common::Keys::Z) {
        sysio->keyup(event.shiftMod ? 'Z' : 'z');
    } else if (event.keyCode == Common::Keys::Num1) {
        sysio->keyup(event.shiftMod ? '!' : '1');
    } else if (event.keyCode == Common::Keys::Num2) {
        sysio->keyup(event.shiftMod ? '@' : '2');
    } else if (event.keyCode == Common::Keys::Num3) {
        sysio->keyup(event.shiftMod ? '#' : '3');
    } else if (event.keyCode == Common::Keys::Num4) {
        sysio->keyup(event.shiftMod ? '$' : '4');
    } else if (event.keyCode == Common::Keys::Num5) {
        sysio->keyup(event.shiftMod ? '%' : '5');
    } else if (event.keyCode == Common::Keys::Num6) {
        sysio->keyup(event.shiftMod ? '^' : '6');
    } else if (event.keyCode == Common::Keys::Num7) {
        sysio->keyup(event.shiftMod ? '&' : '7');
    } else if (event.keyCode == Common::Keys::Num8) {
        sysio->keyup(event.shiftMod ? '*' : '8');
    } else if (event.keyCode == Common::Keys::Num9) {
        sysio->keyup(event.shiftMod ? '(' : '9');
    } else if (event.keyCode == Common::Keys::Num0) {
        sysio->keyup(event.shiftMod ? ')' : '0');
    } else if (event.keyCode == Common::Keys::Backquote) {
        sysio->keyup(event.shiftMod ? '~' : '`');
    } else if (event.keyCode == Common::Keys::LBracket) {
        sysio->keyup(event.shiftMod ? '{' : '[');
    } else if (event.keyCode == Common::Keys::RBracket) {
        sysio->keyup(event.shiftMod ? '}' : ']');
    } else if (event.keyCode == Common::Keys::Semicolon) {
        sysio->keyup(event.shiftMod ? ':' : ';');
    } else if (event.keyCode == Common::Keys::Comma) {
        sysio->keyup(event.shiftMod ? '<' : ',');
    } else if (event.keyCode == Common::Keys::Period) {
        sysio->keyup(event.shiftMod ? '>' : '.');
    } else if (event.keyCode == Common::Keys::Quote) {
        sysio->keyup(event.shiftMod ? '"' : '\'');
    } else if (event.keyCode == Common::Keys::Slash) {
        sysio->keyup(event.shiftMod ? '?' : '/');
    } else if (event.keyCode == Common::Keys::Backslash) {
        sysio->keyup(event.shiftMod ? '|' : '\\');
    } else if (event.keyCode == Common::Keys::Equal) {
        sysio->keyup(event.shiftMod ? '+' : '=');
    } else if (event.keyCode == Common::Keys::Hyphen) {
        sysio->keyup(event.shiftMod ? '_' : '-');
    } else if (event.keyCode == Common::Keys::Enter) {
        sysio->keyup('\n');
    } else if (event.keyCode == Common::Keys::Tab) {
        sysio->keyup('\t');
    } else if (event.keyCode == Common::Keys::Space) {
        sysio->keyup(' ');
    } else if (event.keyCode == Common::Keys::Backspace) {
        sysio->keyup(8);
   }
}
