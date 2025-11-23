/****************************************

	Class Action Source

	(c) by 1994 Salim Gasmi

	File : idcmp.c

	Goal : Handle IDCMP messages

	Ver  : 2.0

****************************************/

#include <exec/types.h>
#include <exec/exec.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <string.h>

#include "gadgets.h"
#include "includes.h"

int changed = 0;
int newa = 0;
static int newc = 0;
char ch[1024];
char file[1024];
struct Class *cla;
struct Action *act, *tmp, *prev;
struct Class *SelectedClass = NULL;
struct Class *Dest = NULL;
struct Action *SelectedAction = NULL;
struct Node *nod;
int i;
int ActualOffset = 0;
int SelectedCode = -1;
int nono;
int DestCode;

void HandleIDCMP(void)
{
	struct IntuiMessage *Msg;
	long Class, Code, ID;
	struct Gadget *SelGadget;
	BOOL ENDTEST = FALSE;
	BOOL CHECKGADGET = TRUE;

	while (!(ENDTEST))
	{
		WaitPort(Test_Wnd->UserPort);
		while (Msg = GT_GetIMsg(Test_Wnd->UserPort))
		{

			Class = Msg->Class;
			Code = Msg->Code;
			if ((Class == IDCMP_GADGETUP) || (Class == IDCMP_GADGETDOWN))
			{

				if (SelGadget = (struct Gadget *)Msg->IAddress)
					ID = SelGadget->GadgetID;
			}
			GT_ReplyIMsg(Msg);

			char *res = NULL;
			switch (Class)
			{
			case IDCMP_GADGETUP:
			case IDCMP_GADGETDOWN:

				if (ID == GD_Action_Copy && SelectedClass != NULL)
				{
					i = 0;
					Info(LocaleMsg[CAP_SELECT]);
					Dest = SelectedClass;
					SelectedClass = MemList.head;
					while (SelectedClass != NULL)
					{
						if (strcmp(SelectedClass->name, Dest->name) == 0)
							DestCode = i;
						i++;
						SelectedClass = SelectedClass->next;
					}

					CopyOn();
				}

				if (ID == GD_Class_Add)
				{
					changed = 1;
					newc++;
					sprintf(ch, LocaleMsg[CAP_NEW_CLASS], newc);
					cla = (struct Class *)malloc(sizeof(struct Class));

					strcpy(cla->name, ch);
					cla->nbaction = 0;
					cla->action = NULL;
					cla->actiontail = NULL;
					cla->next = NULL;
					strcpy(cla->suffix, "");

					for (i = 0; i < 5; i++)
						strcpy(cla->value[i], "");

					if (MemList.tail == NULL)
						MemList.head = MemList.tail = cla;
					else
					{
						MemList.tail->next = cla;
						MemList.tail = cla;
						(MemList.size)++;
					}

					AddList(&Clist, cla->name, 0);
					PutList(GD_Class_List, &Clist);

					PutCycle(GD_Offset_Cycle, 0);
					ActualOffset = 0;

					PutString(GD_Class_Name, cla->name);
					PutString(GD_Class_Ext, cla->suffix);
					PutString(GD_Class_Off, cla->value[0]);
					SelectedClass = cla;
					DisableClass(FALSE);

					FreeList(&Alist);
					act = cla->action;
					while (act != NULL)
					{
						AddList(&Alist, act->name, 0);
						act = act->next;
					}
					newa = cla->nbaction;

					PutList(GD_Action_List, &Alist);
					PutString(GD_Action_Name, " ");
					PutString(GD_Action_Exec, " ");
					PutString(GD_Action_Stack, " ");
					PutString(GD_Action_Delay, " ");
					Code = MemList.size - 1;
					Visible(GD_Class_List, Code);
					ID = GD_Class_List;
				}

				if (ID == GD_Action_Add && SelectedClass != NULL)
				{
					changed = 0;

					/* we search the name of new action */

					newa = SelectedClass->nbaction + 1;

					while (1 == 1)
					{
						sprintf(ch, LocaleMsg[CAP_NEW_ACTION], newa);

						act = SelectedClass->action;
						while (act != NULL)
						{
							if (strcmp(ch, act->name) == 0)
								changed = 1;
							act = act->next;
						}
						if (changed == 1)
						{
							changed = 0;
							newa++;
						}
						else
							break;
					}

					act = (struct Action *)malloc(sizeof(struct Action));

					strcpy(act->name, ch);
					strcpy(act->exec, NOACT);
					strcpy(act->stack, "4096");
					strcpy(act->delay, "-1");
					act->wbrun = 0;
					PutCycle(GD_Action_Wb, 0);
					act->next = NULL;

					if (SelectedClass->action == NULL)
					{
						SelectedClass->action = act;
						SelectedClass->actiontail = act;
						Code = 0;
						ID = GD_Action_List;
					}
					else
					{
						SelectedClass->actiontail->next = act;
						SelectedClass->actiontail = act;
					}
					SelectedClass->nbaction++;

					AddList(&Alist, act->name, 0);
					PutList(GD_Action_List, &Alist);

					SelectedAction = act;
					DisableAction(FALSE);
					Code = SelectedClass->nbaction - 1;
					Visible(GD_Action_List, Code);
					ID = GD_Action_List;
				}

				if (ID == GD_Class_Name && SelectedClass != NULL)
				{
					changed = 1;
					res = ((struct StringInfo *)SelGadget->SpecialInfo)->Buffer;
					if (res)
					{
						strcpy(SelectedClass->name, res);
						PutList(GD_Class_List, &Clist);
					}
				}

				if (ID == GD_Action_Name && SelectedAction != NULL)
				{
					changed = 1;
					res = ((struct StringInfo *)SelGadget->SpecialInfo)->Buffer;
					if (res)
					{
						strcpy(SelectedAction->name, res);
						PutList(GD_Action_List, &Alist);
					}
				}

				if (ID == GD_Action_Exec && SelectedAction != NULL)
				{
					changed = 1;
					res = ((struct StringInfo *)SelGadget->SpecialInfo)->Buffer;
					if (res)
						strcpy(SelectedAction->exec, res);
				}

				if (ID == GD_Offset_Cycle)
				{
					ActualOffset = GetCycle(GD_Offset_Cycle);

					if (SelectedClass != NULL)
						PutString(GD_Class_Off, SelectedClass->value[ActualOffset]);
				}

				if (ID == GD_Class_Off && SelectedClass != NULL)
				{
					changed = 1;
					res = ((struct StringInfo *)SelGadget->SpecialInfo)->Buffer;
					strcpy(SelectedClass->value[ActualOffset], res);
				}

				if (ID == GD_Class_Ext && SelectedClass != NULL)
				{
					changed = 1;
					res = ((struct StringInfo *)SelGadget->SpecialInfo)->Buffer;
					strcpy(SelectedClass->suffix, res);
				}

				if (ID == GD_Class_Del && SelectedClass != NULL)
				{
					nono = 0;
					if (SelectedClass == MemList.head || SelectedClass == &Generic)
					{
						Info(LocaleMsg[CAP_CANT_DELETE]);
						nono = 1;
					}

					if (nono == 0 && Req(LocaleMsg[CAP_DEL_CLASS]) == 1)
					{
						changed = 1;
						nod = FindName(&Clist, SelectedClass->name);
						Remove(nod);
						act = SelectedClass->action;
						while (act != NULL)
						{
							tmp = act->next;
							free(act);
							act = tmp;
						}
						i = 0;

						MemList.size--;

						cla = MemList.head;

						while (cla->next != SelectedClass)
							cla = cla->next;
						if (SelectedClass == MemList.tail)
						{
							cla->next = NULL;
							MemList.tail = cla;
						}
						else
							cla->next = SelectedClass->next;

						free(SelectedClass);
						FreeList(&Alist);
						SelectedClass = MemList.head;

						PutList(GD_Class_List, &Clist);
						PutList(GD_Action_List, &Alist);
						ID = GD_Class_List;
						Code = 0;
						Visible(GD_Class_List, Code);
					}
				}

				if (ID == GD_Action_Wb && SelectedAction != NULL)
				{

					changed = 1;
					i = GetCycle(GD_Action_Wb);

					if (i == 0)
					{
						PutString(GD_Action_Stack, SelectedAction->stack);
						PutString(GD_Action_Delay, SelectedAction->delay);
						Status(GD_Action_Stack, FALSE);
						Status(GD_Action_Delay, FALSE);
					}
					else
					{
						Status(GD_Action_Stack, TRUE);
						Status(GD_Action_Delay, TRUE);
					}

					SelectedAction->wbrun = i;
				}

				if (ID == GD_Action_Stack && SelectedAction != NULL)
				{
					changed = 1;
					res = ((struct StringInfo *)SelGadget->SpecialInfo)->Buffer;
					if (res)
						strcpy(SelectedAction->stack, res);
				}

				if (ID == GD_Action_Delay && SelectedAction != NULL)
				{
					changed = 1;
					res = ((struct StringInfo *)SelGadget->SpecialInfo)->Buffer;
					if (res)
						strcpy(SelectedAction->delay, res);
				}

				if (ID == GD_Action_Del && SelectedAction != NULL)
					if (Req(LocaleMsg[CAP_DEL_ACTION]) == 1)
					{
						changed = 1;
						nod = FindName(&Alist, SelectedAction->name);
						Remove(nod);
						act = SelectedClass->action;
						i = 0;

						if (SelectedAction == SelectedClass->action)
						{
							SelectedClass->action = SelectedAction->next;
							if (SelectedClass->actiontail == SelectedAction)
								SelectedClass->actiontail = NULL;
						}
						else
						{
							while (act->next != SelectedAction)
								act = act->next;
							if (SelectedAction == SelectedClass->actiontail)
							{
								act->next = NULL;
								SelectedClass->actiontail = act;
							}
							else
								act->next = SelectedAction->next;
						}

						free(SelectedAction);
						SelectedClass->nbaction--;

						if (SelectedClass->action == NULL)
						{
							PutString(GD_Action_Name, " ");
							PutString(GD_Action_Exec, " ");
							PutString(GD_Action_Stack, " ");
							PutString(GD_Action_Delay, " ");
							DisableAction(TRUE);
							SelectedCode = -1;
						}
						else
						{
							ID = GD_Action_List;
							Code = 0;
							Visible(GD_Action_List, Code);
						}

						PutList(GD_Action_List, &Alist);
					}

				if (ID == GD_Action_Load && SelectedAction != NULL && Getfile(file) == 1)
				{
					changed = 1;
					i = ((struct StringInfo *)Test_Gadgets[GD_Action_Exec]->SpecialInfo)->BufferPos;
					InserText(GD_Action_Exec, file, i);
				}

				if (ID == GD_Action_Com && SelectedAction != NULL)
				{
					changed = 1;
					i = ((struct StringInfo *)Test_Gadgets[GD_Action_Exec]->SpecialInfo)->BufferPos;
					PopUp(ch);
					if (ch[0] != 0)
						InserText(GD_Action_Exec, ch, i);
				}

				if (ID == GD_Action_Up && SelectedAction != NULL)
				{
					if (SelectedAction != SelectedClass->action)
					{
						tmp = SelectedClass->action;
						while (tmp->next != SelectedAction)
							tmp = tmp->next;

						prev = SelectedClass->action;
						if (prev != tmp)
							while (prev->next != tmp)
								prev = prev->next;

						if (prev == tmp)
						{
							SelectedClass->action = SelectedAction;
							prev->next = SelectedAction->next;
							SelectedAction->next = prev;
						}
						else
						{
							prev->next = SelectedAction;
							tmp->next = SelectedAction->next;
							SelectedAction->next = tmp;
						}

						FreeList(&Alist);
						tmp = SelectedClass->action;
						while (tmp != NULL)
						{
							AddList(&Alist, tmp->name, 0);
							if (tmp->next == NULL)
								SelectedClass->actiontail = tmp;
							tmp = tmp->next;
						}

						PutList(GD_Action_List, &Alist);
						SelectedCode--;
						Select(GD_Action_List, SelectedCode);
					}
				}

				if (ID == GD_Action_Down && SelectedAction != NULL)
				{
					if (SelectedAction != SelectedClass->actiontail)
					{
						tmp = SelectedClass->action;
						if (tmp != SelectedAction)
							while (tmp->next != SelectedAction)
								tmp = tmp->next;

						prev = SelectedAction->next;

						if (tmp == SelectedAction)
						{
							prev = SelectedAction->next;
							SelectedClass->action = prev;
							tmp->next = prev->next;
							prev->next = tmp;
						}
						else
						{
							tmp->next = prev;
							SelectedAction->next = prev->next;
							prev->next = SelectedAction;
						}

						FreeList(&Alist);
						tmp = SelectedClass->action;
						while (tmp != NULL)
						{
							AddList(&Alist, tmp->name, 0);
							if (tmp->next == NULL)
								SelectedClass->actiontail = tmp;
							tmp = tmp->next;
						}

						PutList(GD_Action_List, &Alist);
						SelectedCode++;
						Select(GD_Action_List, SelectedCode);
					}
				}

				if (ID == GD_Proj_Save)
				{
					SaveMem(1);
					ENDTEST = TRUE;
				}

				if (ID == GD_Class_Learn)
					if (SelectedClass != NULL && SelectedClass != MemList.head && SelectedClass != &Generic)
					{
						Learn(SelectedClass);
					}

				if (ID == GD_Proj_About)
				{
					sprintf(ch, LocaleMsg[CAP_COPYRIGHT], Ver);
					Info(ch);
				}

				if (ID == GD_Proj_Quit)
				{
					SaveMem(0);
					ENDTEST = TRUE;
				}

				if (ID == GD_Proj_Cancel)
				{
					ENDTEST = TRUE;
				}

				if (ID == GD_Class_List)
				{
					cla = MemList.head;
					for (i = 0; i < Code; i++)
						cla = cla->next;

					SelectedClass = cla;
					Select(GD_Class_List, Code);

					if (Dest != NULL && Dest != cla)
					{
						act = cla->action;
						while (act != NULL)
						{
							changed = 1;
							tmp = (struct Action *)malloc(sizeof(struct Action));

							strcpy(tmp->name, act->name);
							strcpy(tmp->exec, act->exec);
							strcpy(tmp->stack, act->stack);
							tmp->wbrun = act->wbrun;
							strcpy(tmp->delay, act->delay);
							tmp->next = NULL;

							if (Dest->action == NULL)
							{
								Dest->action = tmp;
								Dest->actiontail = tmp;
							}
							else
							{
								Dest->actiontail->next = tmp;
								Dest->actiontail = tmp;
							}
							Dest->nbaction++;
							act = act->next;
						}

						SelectedClass = Dest;
						cla = Dest;
						Dest = NULL;
						Select(GD_Class_List, DestCode);
						CopyOff();
					}

					if (Dest != cla)
					{
						PutCycle(GD_Offset_Cycle, 0);
						ActualOffset = 0;

						PutString(GD_Class_Name, cla->name);
						PutString(GD_Class_Ext, cla->suffix);
						PutString(GD_Class_Off, cla->value[0]);
						DisableClass(FALSE);

						FreeList(&Alist);
						act = cla->action;
						while (act != NULL)
						{
							AddList(&Alist, act->name, 0);
							act = act->next;
						}
						newa = cla->nbaction;

						PutList(GD_Action_List, &Alist);
						PutString(GD_Action_Name, " ");
						PutString(GD_Action_Exec, " ");
						PutString(GD_Action_Stack, " ");
						PutString(GD_Action_Delay, " ");
						if (SelectedClass->action == NULL)
						{
							DisableAction(TRUE);
							SelectedCode = -1;
						}
						else
						{
							Code = 0;
							ID = GD_Action_List;
							Select(GD_Action_List, Code);
						}
						if (SelectedClass == MemList.head)
						{
							Status(GD_Class_Off, 1);
							Status(GD_Class_Ext, 1);
						}
						if (SelectedClass == &Generic)
							DisableClass(TRUE);
					}
					else
						Info(LocaleMsg[CAP_UNABLE]);
				}

				if (ID == GD_Action_List)
				{
					act = SelectedClass->action;
					for (i = 0; i < Code; i++)
						act = act->next;
					SelectedAction = act;
					SelectedCode = Code;

					Select(GD_Action_List, Code);

					PutString(GD_Action_Name, act->name);
					PutString(GD_Action_Exec, act->exec);
					DisableAction(FALSE);

					if (act->wbrun == 0)
					{
						PutCycle(GD_Action_Wb, 0);
						PutString(GD_Action_Stack, act->stack);
						PutString(GD_Action_Delay, act->delay);
						Status(GD_Action_Stack, FALSE);
						Status(GD_Action_Delay, FALSE);
					}
					else
					{
						PutCycle(GD_Action_Wb, act->wbrun);
						Status(GD_Action_Stack, TRUE);
						Status(GD_Action_Delay, TRUE);
					}
				}

				break;

			default:
				break;
			}
		}
	}

	Forbid();

	while (Msg = GT_GetIMsg(Test_Wnd->UserPort))
		GT_ReplyIMsg(Msg);

	ModifyIDCMP(Test_Wnd, 0L);

	Permit();
}
