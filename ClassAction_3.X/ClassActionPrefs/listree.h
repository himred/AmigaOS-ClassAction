/****************************************

    Class Action Source

    (c) by 1995 Salim Gasmi

    File : listree.h

    Goal : define listree MUI Custom Class

    Ver  : 3.00

****************************************/

static struct MUI_CustomClass *cl_ddlisttree;

struct Data_DDListtree
{
    Object *string;
    struct MUIS_Listtree_TreeNode *drop;
    UWORD drop_flags;
    LONG old_entry;
};

SAVEDS ASM ULONG Dispatcher_DDListtree(REG(a0) struct IClass *cl, REG(a2) Object *obj, REG(a1) Msg msg)
{
    struct Data_DDListtree *data = INST_DATA(cl, obj);

    switch (msg->MethodID)
    {
    case MUIM_DragQuery:
    {
        struct MUIP_DragQuery *d = (struct MUIP_DragQuery *)msg;

        if (d->obj == data->string)
            return (MUIV_DragQuery_Accept);
    }
    break;

    case MUIM_DragReport:
    {
        struct MUIP_DragReport *d = (struct MUIP_DragReport *)msg;

        if (d->obj == data->string)
        {
            struct MUIS_Listtree_TestPos_Result r;
            struct MUIS_Listtree_TreeNode *tn;

            DoMethod(obj, MUIM_Listtree_TestPos, d->x, d->y, &r);

            if ((r.ListEntry != data->old_entry) || (r.Flags != data->drop_flags))
            {
                if (!d->update)
                    return (MUIV_DragReport_Refresh);

                data->old_entry = r.ListEntry;
                data->drop_flags = r.Flags;

                if (tn = r.TreeNode)
                {
                    /*** if entry to drop has a list ***/
                    if ((tn->tn_Flags & TNF_LIST) || (r.Flags != MUIV_Listtree_TestPos_Result_Flags_Onto))
                    {
                        data->drop = tn;
                        DoMethod(obj, MUIM_Listtree_SetDropMark, r.ListEntry, r.Flags);
                    }
                    else
                    {
                        DoMethod(obj, MUIM_Listtree_SetDropMark, r.ListEntry, MUIV_Listtree_SetDropMark_Values_None);
                        data->drop = NULL;
                    }
                }
                else
                {
                    DoMethod(obj, MUIM_Listtree_SetDropMark, r.ListEntry, MUIV_Listtree_SetDropMark_Values_None);
                    data->drop = NULL;
                }
            }
        }
    }
    break;

    case MUIM_DragDrop:
    {
        struct MUIP_DragDrop *d = (struct MUIP_DragDrop *)msg;

        /*** object valid ? ***/
        if (d->obj == data->string)
        {
            char *name;

            get(d->obj, MUIA_String_Contents, &name);

            if (data->drop)
            {
                switch (data->drop_flags)
                {
                case MUIV_Listtree_TestPos_Result_Flags_Above:
                    DoMethod(obj, MUIM_Listtree_Insert, name, NULL, NULL, data->drop, MUIV_Listtree_Insert_Flags_NextNode);
                    break;

                case MUIV_Listtree_TestPos_Result_Flags_Below:
                    DoMethod(obj, MUIM_Listtree_Insert, name, NULL, NULL, data->drop, 0);
                    break;

                case MUIV_Listtree_TestPos_Result_Flags_Onto:
                    DoMethod(obj, MUIM_Listtree_Insert, name, NULL, data->drop, MUIV_Listtree_Insert_PrevNode_Sorted, 0);
                    break;
                }
            }

            return (0);
        }
    }
    break;
    }

    return (DoSuperMethodA(cl, obj, msg));
}
