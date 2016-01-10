//TEXT_SOURCE_CUSTOM_START=-20000, TEXT_SOURCE_CUSTOM_END=-30000

//#define PR_HH(chrText)  pr_hh(chrText)
#ifndef __PR_LANGUAGE_H
#define __PR_LANGUAGE_H

enum PrStrings
{
	GO_TELE_TO_DALARAN_CRYSTAL_FAILED = -28001,
	GOSSIP_ITEM_MOONSTONE = -28002,
};
#endif

/*
precompiled.h添加
#include "pr_hh.h"

程序使用
#include "precompiled.h"
pPlayer->GetSession()->SendNotification(GO_TELE_TO_DALARAN_CRYSTAL_FAILED);

数据库脚本：
insert into custom_texts(entry, content_default, content_loc4, content_loc5)values
(-28001, 'This teleport crystal cannot be used until the teleport crystal in Dalaran has been used at least once.', '这个传送水晶只能在达拉然使用过之后才能开启。', '这个传送水晶只能在达拉然使用过之后才能开启。');

insert into custom_texts(entry, content_default, content_loc4, content_loc5)values
(-28002, 'Hand over the Southfury moonstone and I\'ll let you go.', '交出月亮石我就放了你。', '交出月亮石我就放了你。');

*/