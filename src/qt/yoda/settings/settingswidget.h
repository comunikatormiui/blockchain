// Copyright (c) 2019 The YODA developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include "qt/yoda/pwidget.h"
#include "qt/yoda/settings/settingsbackupwallet.h"
#include "qt/yoda/settings/settingsbittoolwidget.h"
#include "qt/yoda/settings/settingssignmessagewidgets.h"
#include "qt/yoda/settings/settingswalletrepairwidget.h"
#include "qt/yoda/settings/settingswalletoptionswidget.h"
#include "qt/yoda/settings/settingsmainoptionswidget.h"
#include "qt/yoda/settings/settingsdisplayoptionswidget.h"
#include "qt/yoda/settings/settingsmultisendwidget.h"
#include "qt/yoda/settings/settingsinformationwidget.h"
#include "qt/yoda/settings/settingsconsolewidget.h"

class YODAGUI;

QT_BEGIN_NAMESPACE
class QDataWidgetMapper;
QT_END_NAMESPACE

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public PWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(YODAGUI* parent);
    ~SettingsWidget();

    void loadClientModel() override;
    void loadWalletModel() override;
    void setMapper();
    void showDebugConsole();

Q_SIGNALS:
    /** Get restart command-line parameters and handle restart */
    void handleRestart(QStringList args);

private Q_SLOTS:
    // File
    void onFileClicked();
    void onBackupWalletClicked();
    void onSignMessageClicked();

    // Wallet Configuration
    void onConfigurationClicked();
    void onBipToolClicked();
    void onMultisendClicked();

    // Options
    void onOptionsClicked();
    void onMainOptionsClicked();
    void onWalletOptionsClicked();
    void onDisplayOptionsClicked();

    void onDiscardChanges();

    // Tools
    void onToolsClicked();
    void onInformationClicked();
    void onDebugConsoleClicked();
    void onWalletRepairClicked();

    // Help
    void onHelpClicked();
    void onAboutClicked();

    void onResetAction();
    void onSaveOptionsClicked();
private:
    Ui::SettingsWidget *ui;

    SettingsBackupWallet *settingsBackupWallet;
    SettingsBitToolWidget *settingsBitToolWidget;
    SettingsSignMessageWidgets *settingsSingMessageWidgets;
    SettingsWalletRepairWidget *settingsWalletRepairWidget;
    SettingsWalletOptionsWidget *settingsWalletOptionsWidget;
    SettingsMainOptionsWidget *settingsMainOptionsWidget;
    SettingsDisplayOptionsWidget *settingsDisplayOptionsWidget;
    SettingsMultisendWidget *settingsMultisendWidget;
    SettingsInformationWidget *settingsInformationWidget;
    SettingsConsoleWidget *settingsConsoleWidget;

    QDataWidgetMapper* mapper;

    QList<QPushButton*> options;

    void selectOption(QPushButton* option);
    bool openStandardDialog(QString title = "", QString body = "", QString okBtn = "OK", QString cancelBtn = "");
};

#endif // SETTINGSWIDGET_H
