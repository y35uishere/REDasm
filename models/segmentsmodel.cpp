#include "segmentsmodel.h"
#include <core/plugins/loader.h>
#include <QColor>
#include "../themeprovider.h"

#define ADD_SEGMENT_TYPE(s, t) { if(!s.isEmpty()) s += " | ";  s += t; }

SegmentsModel::SegmentsModel(QObject *parent) : ListingItemModel(REDasm::ListingItem::SegmentItem, parent) { }

QVariant SegmentsModel::data(const QModelIndex &index, int role) const
{
    if(!m_disassembler)
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        const REDasm::AssemblerPlugin* assembler = m_disassembler->assembler();
        const REDasm::Segment& segment = m_disassembler->document()->segments().at(index.row());

        if(index.column() == 0)
            return S_TO_QS(REDasm::hex(segment.address, assembler->bits()));
        if(index.column() == 1)
            return S_TO_QS(REDasm::hex(segment.endaddress, assembler->bits()));
        if(index.column() == 2)
            return S_TO_QS(REDasm::hex(segment.size(), assembler->bits()));
        if(index.column() == 3)
            return S_TO_QS(REDasm::hex(segment.offset, assembler->bits()));
        if(index.column() == 4)
            return S_TO_QS(REDasm::hex(segment.endoffset, assembler->bits()));
        if(index.column() == 5)
            return S_TO_QS(REDasm::hex(segment.rawSize(), assembler->bits()));
        if(index.column() == 6)
            return S_TO_QS(segment.name);
        if(index.column() == 7)
            return SegmentsModel::segmentFlags(&segment);
    }
    else if(role == Qt::ForegroundRole)
    {
        if(index.column() == 6)
            return THEME_VALUE("segment_name_fg");
        else if(index.column() == 7)
            return THEME_VALUE("segment_flags_fg");

        return THEME_VALUE("address_list_fg");
    }
    else if(role == Qt::TextAlignmentRole)
    {
        if(index.column() > 5)
            return Qt::AlignCenter;
        else
            return Qt::AlignRight;
    }

    return QVariant();
}

QVariant SegmentsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical || role != Qt::DisplayRole)
        return ListingItemModel::headerData(section, orientation, role);

    if(section == 0)
        return "Start Address";
    if(section == 1)
        return "End Address";
    if(section == 2)
        return "Size";
    if(section == 3)
        return "Offset";
    if(section == 4)
        return "End Offset";
    if(section == 5)
        return "Raw Size";
    if(section == 6)
        return "Name";
    if(section == 7)
        return "Type";

    return ListingItemModel::headerData(section, orientation, role);
}

int SegmentsModel::columnCount(const QModelIndex &) const { return 8; }

QString SegmentsModel::segmentFlags(const REDasm::Segment *segment)
{
    QString s;

    if(segment->is(REDasm::SegmentType::Code))
        ADD_SEGMENT_TYPE(s, "CODE")

    if(segment->is(REDasm::SegmentType::Data))
        ADD_SEGMENT_TYPE(s, "DATA")

    if(segment->is(REDasm::SegmentType::Bss))
        ADD_SEGMENT_TYPE(s, "BSS")

    return s;
}
