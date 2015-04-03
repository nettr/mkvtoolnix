#include "common/common_pch.h"

#include <QFileInfo>

#include <matroska/KaxInfoData.h>
#include <matroska/KaxSemantic.h>

#include "common/qt.h"
#include "mkvtoolnix-gui/forms/header_editor/tab.h"
#include "mkvtoolnix-gui/header_editor/bit_value_page.h"
#include "mkvtoolnix-gui/header_editor/page_model.h"
#include "mkvtoolnix-gui/header_editor/string_value_page.h"
#include "mkvtoolnix-gui/header_editor/tab.h"
#include "mkvtoolnix-gui/header_editor/top_level_page.h"

namespace mtx { namespace gui { namespace HeaderEditor {

using namespace mtx::gui;

Tab::Tab(QWidget *parent,
         QString const &fileName,
         std::unique_ptr<QtKaxAnalyzer> &&analyzer)
  : QWidget{parent}
  , ui{new Ui::Tab}
  , m_fileName{fileName}
  , m_analyzer{std::move(analyzer)}
  , m_model{new PageModel{this, *m_analyzer}}
{
  // Setup UI controls.
  ui->setupUi(this);

  setupUi();

  populateTree();
}

Tab::~Tab() {
}

void
Tab::setupUi() {
  auto info = QFileInfo{m_fileName};
  ui->fileName->setText(info.fileName());
  ui->directory->setText(info.path());

  ui->elements->setModel(m_model);

  m_pageContainerLayout = new QVBoxLayout{ui->pageContainer};
}

void
Tab::appendPage(PageBase *page,
                QModelIndex const &parentIdx) {
  m_pageContainerLayout->addWidget(page);
  m_model->appendPage(page, parentIdx);
}

PageModel *
Tab::getModel()
  const {
  return m_model;
}

void
Tab::retranslateUi() {
  ui->fileNameLabel->setText(QY("File name:"));
  ui->directoryLabel->setText(QY("Directory:"));

  auto &pages = m_model->getPages();
  for (auto const &page : pages)
    page->retranslateUi();
}

void
Tab::populateTree() {
  for (auto &data : m_analyzer->m_data)
    if (data->m_id == KaxInfo::ClassInfos.GlobalId) {
      handleSegmentInfo(*data);
      break;
    }

  for (auto &data : m_analyzer->m_data)
    if (data->m_id == KaxTracks::ClassInfos.GlobalId) {
      handleTracks(*data);
      break;
    }

  m_analyzer->close_file();
}

void
Tab::itemSelected(QModelIndex idx) {
  auto selectedPage = m_model->selectedPage(idx);
  if (!selectedPage)
    return;

  auto pages = m_model->getPages();
  for (auto const &page : pages)
    page->setVisible(page == selectedPage);
}

QWidget *
Tab::getPageContainer()
  const {
  return ui->pageContainer;
}

void
Tab::handleSegmentInfo(kax_analyzer_data_c &data) {
  m_eSegmentInfo = m_analyzer->read_element(&data);
  if (!m_eSegmentInfo)
    return;

  auto &info = static_cast<KaxInfo &>(*m_eSegmentInfo.get());
  auto page  = new TopLevelPage{*this, YT("Segment information"), m_eSegmentInfo};
  page->init();

  (new StringValuePage{*this, *page, info, KaxTitle::ClassInfos,           YT("Title"),                        YT("The title for the whole movie.")})->init();
  (new StringValuePage{*this, *page, info, KaxSegmentFilename::ClassInfos, YT("Segment file name"),            YT("The file name for this segment.")})->init();
  (new StringValuePage{*this, *page, info, KaxPrevFilename::ClassInfos,    YT("Previous file name"),           YT("An escaped file name corresponding to the previous segment.")})->init();
  (new StringValuePage{*this, *page, info, KaxNextFilename::ClassInfos,    YT("Next filename"),                YT("An escaped file name corresponding to the next segment.")})->init();
  (new BitValuePage{   *this, *page, info, KaxSegmentUID::ClassInfos,      YT("Segment unique ID"),            YT("A randomly generated unique ID to identify the current segment between many others (128 bits)."), 128})->init();
  (new BitValuePage{   *this, *page, info, KaxPrevUID::ClassInfos,         YT("Previous segment's unique ID"), YT("A unique ID to identify the previous chained segment (128 bits)."), 128})->init();
  (new BitValuePage{   *this, *page, info, KaxNextUID::ClassInfos,         YT("Next segment's unique ID"),     YT("A unique ID to identify the next chained segment (128 bits)."), 128})->init();
}

void
Tab::handleTracks(kax_analyzer_data_c &data) {
  m_eTracks = m_analyzer->read_element(&data);
  if (!m_eTracks)
    return;

  // auto kaxTracks = static_cast<KaxTracks *>(m_eTracks.get());

  // TODO: Tab::handleTracks
}

}}}
