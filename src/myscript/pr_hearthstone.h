const static int longMountSpell[60] = {//Áú
	3363,
	8395,
	10795,
	10796,
	10798,
	10799,
	16084,
	17450,
	22721,
	23241,
	23242,
	23243,
	24242,
	24576,
	28828,
	32420,
	37015,
	40212,
	41513,
	41514,
	41515,
	41516,
	41517,
	41518,
	43810,
	44317,
	44744,
	49193,
	51960,
	58615,
	59567,
	59568,
	59569,
	59570,
	59571,
	59650,
	59961,
	59976,
	59996,
	60002,
	60021,
	60024,
	60025,
	61294,
	61996,
	61997,
	62048,
	63635,
	63956,
	63963,
	64659,
	64927,
	65439,
	65644,
	66088,
	67336,
	68769,
	69395,
	71810,
	34068
};
const static int fengMountSpell[1] = {//·ï
	32345
};
const static int tanMountSpell[7] = {//Ìº
	61309,
	61442,
	61444,
	61446,
	61451,
	75387,
	75596
};
const static int shijiuMountSpell[15] = {//Ê¨ðÕ
	32235,
	32239,
	32240,
	32242,
	32289,
	32290,
	32292,
	54726,
	61229,
	64681,
	43927,
	63844,
	66087,
	74854,
	74855

};
const static int xiangMountSpell[20] = {//Ïó
	34406,
	34407,
	35710,
	35711,
	35712,
	35713,
	35714,
	47037,
	59785,
	59788,
	59791,
	59793,
	59797,
	59799,
	59802,
	59804,
	60136,
	60140,
	63639,
	65637
};
const static int xiongMountSpell[12] = {//ÐÜ
	43688,
	51412,
	54753,
	55293,
	58983,
	59572,
	59573,
	60114,
	60116,
	60118,
	60119,
	64992
};
const static int jiMountSpell[3] = {//¼¦
	65917,
	66123,
	66124
};
const static int huMountSpell[3] = {//»¢
	24252,
	42776,
	42777
};
const static int luMountSpell[2] = { //Â¹
	25675,
	44655
};

const static int tankeMountSpell[2] = { //³æ
	25863,
	31700
};
const static int shouMountSpell[15] = { //ÊÞ
	18363,
	18989,
	18990,
	18991,
	18992,
	22718,
	23247,
	23248,
	23249,
	49378,
	49379,
	50869,
	63641,
	64657,
	65641
};
const static int niaoMountSpell[36] = { //Äñ
	8396,
	10800,
	10801,
	10802,
	10803,
	10804,
	10873,
	10969,
	15779,
	15780,
	15781,
	17453,
	17454,
	17455,
	17456,
	17458,
	17459,
	22719,
	23222,
	23223,
	23225,
	33630,
	33660,
	34795,
	35018,
	35020,
	35022,
	35025,
	35027,
	35028,
	46628,
	63638,
	63642,
	65639,
	65642,
	66091
};
const static int langMountSpell[18] = { //ÀÇ
	459,
	578,
	579,
	580,
	581,
	6653,
	6654,
	16080,
	16081,
	22724,
	23250,
	23251,
	23252,
	23509,
	63640,
	64658,
	65646,
	68056
};

const static int baoMountSpell[21] = { //±ª
	8394,
	10787,
	10788,
	10789,
	10790,
	10792,
	10793,
	16055,
	16056,
	16058,
	16059,
	16060,
	17229,
	22723,
	23219,
	23220,
	23221,
	23338,
	63637,
	65638,
	66847
};
const static int maMountSpell[40] = { //Âí
	458,
	468,
	470,
	471,
	472,
	6648,
	8980,
	16082,
	16083,
	17462,
	17463,
	17464,
	17465,
	17481,
	22722,
	23227,
	23228,
	23229,
	23246,
	29059,
	36702,
	48778,
	48954,
	49322,
	58819,
	63232,
	63643,
	64656,
	64977,
	65640,
	65645,
	66090,
	66846,
	68057,
	68187,
	68188,
	68768,
	73313,
	75617,
	75618
};

const static int yangMountSpell[27] = { //Ñò
	6777,
	6896,
	6897,
	6898,
	6899,
	17460,
	17461,
	22720,
	23238,
	23239,
	23240,
	34790,
	34896,
	34897,
	34898,
	34899,
	39315,
	39316,
	39317,
	39318,
	39319,	
	43899,
	43900,
	46980,
	50870,
	63636,
	65643
};
bool hearthstone_random_mount(Player* pPlayer, const int spellarray[]);
void hearthstone_mount(Player* pPlayer, Item* pItem, uint32 uiAction);
void hearthstone_prepare_mount(Player* pPlayer, Item* pItem);
bool levelup(Player* pPlayer, int level, int point);
void hearthstone_prepare_gamedirect(Player* pPlayer, Item* pItem);
void hearthstone_gamedirect(Player* pPlayer, Item* pItem, uint32 uiAction);
void hearthstone_learn_professional(Player* pPlayer, Item* pItem, uint32 uiAction);
bool learn_professional(Player* pPlayer, uint32 skillid, uint8 maxnum);
bool explorecheat(Player* pPlayer);
bool taxicheat(Player* pPlayer);
bool learn_default_spell(Player* pPlayer, Item* pItem, uint32 uiAction);

bool hearthstone_prepare_transport2(Player* pPlayer, Item* pItem, uint32 uiAction);
bool hearthstone_transport(Player* pPlayer, uint32 mapid, double x, double y, double z, double o);
bool hearthstone_transport_case(Player* pPlayer, Item* pItem, uint32 uiAction);
bool hearthstone_itemset(Player* pPlayer, Item* pItem, uint32 uiAction);
void hearthstone_prepare_itemset(Player* pPlayer, Item* pItem, uint32 uiAction);
bool additemset(Player* pPlayer, uint32 itemset);
void hearthstone_prepare_quest(Player* pPlayer, Item* pItem);
bool hearthstone_click2(Player* pPlayer, Item* pItem);
void hearthstone_quest(Player* pPlayer, Item* pItem, uint32 questid);
void transportByCreatureOrGO(Player* pPlayer, Item* pItem, int idx);
void transportByPOI(Player* pPlayer, Item* pItem, int idx);
void transportByCreatureOrGO2(Player* pPlayer, Item* pItem, int32 creatureOrGOEntry);
void transportToStarter(Player* pPlayer, Item* pItem);
void transportToInvolved(Player* pPlayer, Item* pItem, int idx);