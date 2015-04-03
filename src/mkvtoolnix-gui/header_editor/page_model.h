#ifndef MTX_MKVTOOLNIX_GUI_HEADER_EDITOR_PAGE_MODEL_H
#define MTX_MKVTOOLNIX_GUI_HEADER_EDITOR_PAGE_MODEL_H

#include "common/common_pch.h"

#include <QList>
#include <QStandardItemModel>

#include "common/qt_kax_analyzer.h"

class QAbstractItemView;

namespace mtx { namespace gui { namespace HeaderEditor {

class PageBase;

class PageModel: public QStandardItemModel {
  Q_OBJECT;
protected:
  QList<PageBase *> m_pages, m_topLevelPages;
  QtKaxAnalyzer &m_analyzer;

public:
  PageModel(QObject *parent, QtKaxAnalyzer &analyzer);
  virtual ~PageModel();

  PageBase *selectedPage(QModelIndex const &idx) const;

  void appendPage(PageBase *page, QModelIndex const &parentIdx = {});

  QList<PageBase *> &getPages();

public slots:

protected:
};

}}}

#endif  // MTX_MKVTOOLNIX_GUI_HEADER_EDITOR_PAGE_MODEL_H
