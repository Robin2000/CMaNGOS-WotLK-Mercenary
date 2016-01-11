//TEXT_SOURCE_CUSTOM_START=-20000, TEXT_SOURCE_CUSTOM_END=-30000

//#define PR_HH(chrText)  pr_hh(chrText)
#ifndef __PR_LANGUAGE_H
#define __PR_LANGUAGE_H

enum PrStrings
{
	GO_TELE_TO_DALARAN_CRYSTAL_FAILED = -2800001,
	GOSSIP_ITEM_MOONSTONE = -2800002,
};
#endif

/*
precompiled.h添加
#include "pr_hh.h"

程序使用
#include "precompiled.h"
pPlayer->GetSession()->SendNotification(GO_TELE_TO_DALARAN_CRYSTAL_FAILED);

数据库脚本：
delete from custom_texts where entry<=-2800001 and entry>=2800002; 

insert into custom_texts(entry, content_default, content_loc4, content_loc5)values
(-2800001, 'This teleport crystal cannot be used until the teleport crystal in Dalaran has been used at least once.', '这个传送水晶只能在达拉然首次使用过之后才能开启。', '这个传送水晶只能在达拉然首次使用过之后才能开启。');

insert into custom_texts(entry, content_default, content_loc4, content_loc5)values
(-2800002, 'Hand over the Southfury moonstone and I\'ll let you go.', '交出月亮石我就放了你。', '交出月亮石我就放了你。');

*/