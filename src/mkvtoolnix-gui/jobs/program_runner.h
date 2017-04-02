#ifndef MTX_MKVTOOLNIX_GUI_JOBS_PROGRAM_RUNNER_H
#define MTX_MKVTOOLNIX_GUI_JOBS_PROGRAM_RUNNER_H

#include "common/common_pch.h"

#include "mkvtoolnix-gui/util/settings.h"

namespace mtx { namespace gui {

namespace Jobs {

class Job;
enum class QueueStatus;

class ProgramRunner: public QObject {
  Q_OBJECT;

public:
  using VariableMap = QMap<QString, QStringList>;

  enum class ExecuteActionCondition {
    AfterJobFinishes,
    AfterQueueFinishes,
  };

protected:
  QMap<ExecuteActionCondition, QSet<Util::Settings::RunProgramConfig *>> m_actionsToExecute;

public:
  explicit ProgramRunner();
  virtual ~ProgramRunner();

  virtual void setup();
  virtual bool isRunProgramTypeSupported(Util::Settings::RunProgramType type);

  virtual void enableActionToExecute(Util::Settings::RunProgramConfig &config, ExecuteActionCondition condition, bool enable);
  virtual bool isActionToExecuteEnabled(Util::Settings::RunProgramConfig &config, ExecuteActionCondition condition);

  virtual void run(Util::Settings::RunProgramForEvent forEvent, std::function<void(VariableMap &)> const &setupVariables, Util::Settings::RunProgramConfigPtr const &forceRunThis = Util::Settings::RunProgramConfigPtr{});

public slots:
  virtual void executeActionsAfterJobFinishes(Job const &job);
  virtual void executeActionsAfterQueueFinishes(Jobs::QueueStatus status);

protected:
  virtual void executeActions(ExecuteActionCondition condition, Job const *job = nullptr);
  virtual void executeProgram(Util::Settings::RunProgramConfig &config, std::function<void(VariableMap &)> const &setupVariables, VariableMap const &generalVariables);
  virtual void playAudioFile(Util::Settings::RunProgramConfig &config);
  virtual void shutDownComputer(Util::Settings::RunProgramConfig &config);
  virtual void suspendComputer(Util::Settings::RunProgramConfig &config);

public:
  static std::unique_ptr<ProgramRunner> create();

protected:
  static void setupGeneralVariables(VariableMap &variables);
  static QStringList replaceVariables(QStringList const &commandLine, VariableMap const &variables);
};

}}}

#endif  // MTX_MKVTOOLNIX_GUI_JOBS_PROGRAM_RUNNER_H
