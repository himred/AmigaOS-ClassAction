/****************************************

	ClassActionPrefs Source

	(c) by 1995 Salim Gasmi

	File : gadgets.h

	Goal : define gadgets Id

	Ver  : 2.0

****************************************/

#define GetString(g) (((struct StringInfo *)g->SpecialInfo)->Buffer)
#define GetNumber(g) (((struct StringInfo *)g->SpecialInfo)->LongInt)

#define GD_Offset_Cycle 0
#define GD_Class_Add 1
#define GD_Class_Del 2
#define GD_Action_Add 3
#define GD_Action_Del 4
#define GD_Proj_Save 5
#define GD_Proj_Quit 6
#define GD_Proj_About 7
#define GD_Class_List 8
#define GD_Action_List 9
#define GD_Class_Name 10
#define GD_Class_Off 11
#define GD_Class_Ext 12
#define GD_Action_Name 13
#define GD_Action_Exec 14
#define GD_Action_Load 15
#define GD_Action_Wb 16
#define GD_Action_Stack 17
#define GD_Action_Com 18
#define GD_Action_Copy 19
#define GD_Action_Up 20
#define GD_Action_Down 21
#define GD_Proj_Cancel 22
#define GD_Action_Delay 23
#define GD_Class_Learn 24

#define Test__CNT 25
