#ifndef WEBAPP_H
#define WEBAPP_H

#include <QJsonObject>
#include <QJsonValue>

#include <lib/qfrontdesk/qfrontdesk.h>

#include <model/manager/hotkeysmanager.h>

class Webapp : public QFrontDesk
{
    Q_OBJECT
public:
    explicit Webapp(int port, int slots_max, QString static_serve_path = "", QObject *parent = 0);

    HotkeysManager* hotkeys;

    QString routing(QString route, Arguments *arg);

    void setHotkeysManager(HotkeysManager *hotkeys);

    QJsonObject getButtonsForManager(QList<YonderPushButton *> manager_buttons);
    QJsonObject getButtonsStatesForManager(QList<YonderPushButton *> manager_buttons);
    QJsonObject getButtonsForManager(QList<HotkeysPushButton *> manager_buttons);
    QJsonObject getButtonsStatesForManager(QList<HotkeysPushButton *> manager_buttons);
    QVariant toJsonHelper(QJsonObject buttonlist);

    QString routeRoot(Arguments *arg);
    QString routeHotkeys(Arguments *arg);
    QString routeAtmosphere(Arguments *arg);
    QString routeSingleshots(Arguments *arg);
    QString routeSfx(Arguments *arg);
    QString routeJson(Arguments *arg);
    QString routeJsonState(Arguments *arg);


signals:

public slots:

};

#endif // WEBAPP_H

//#ifndef WEBAPP_BUTTON_H
//#define WEBAPP_BUTTON_H

//class WebappButton : public QObject
//{
//  Q_OBJECT
//  Q_PROPERTY(QString label READ label)
//  Q_PROPERTY(QString pos READ pos)
//public:
//  WebappButton(QString label, QString pos, QObject *parent = 0)
//      : QObject(parent), m_label(label), m_pos(pos)
//  {}

//  QString label() { return m_label; }
//  QString pos() { return m_pos; }

//private:
//  QString m_label;
//  QString m_pos;
//};

//#endif
