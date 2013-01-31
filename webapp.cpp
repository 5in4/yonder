#include "webapp.h"

Webapp::Webapp(int port, int slots_max, QString static_serve_path, QObject *parent) :
    QFrontDesk(port, slots_max, static_serve_path, parent)
{
    //setTemplateDirs(QStringList() << ":webservice/");
}

QString Webapp::routing(QString route, Arguments *arg) {
    if(route == "/") {
        return routeRoot(arg);
    }

    if(route == "/button/hotkeys") {
        return routeHotkeys(arg);
    }

    if(route == "/button/atmosphere") {
        return routeAtmosphere(arg);
    }

    if(route == "/button/singleshots") {
        return routeSingleshots(arg);
    }

    if(route == "/button/sfx") {
        return routeSfx(arg);
    }

    if(route == "/json") {
        return routeJson(arg);
    }

    if(route == "/json-state") {
        return routeJsonState(arg);
    }
    return 0;
}


void Webapp::setHotkeysManager(HotkeysManager *hotkeys) {
    this->hotkeys = hotkeys;
}


QJsonObject Webapp::getButtonsForManager(QList<YonderPushButton *> manager_buttons) {
    QJsonObject buttons;
    for(int i=0; i < manager_buttons.length(); i++) {
        buttons.insert(QString("%1").arg(manager_buttons.at(i)->getPosInArray()), QJsonValue(manager_buttons.at(i)->label()));
    }
    return buttons;
}

QJsonObject Webapp::getButtonsForManager(QList<HotkeysPushButton *> manager_buttons) {
    QJsonObject buttons;
    for(int i=0; i < manager_buttons.length(); i++) {
        buttons.insert(QString("%1").arg(manager_buttons.at(i)->getPosInArray()), QJsonValue(manager_buttons.at(i)->label()));
    }
    return buttons;
}



QJsonObject Webapp::getButtonsStatesForManager(QList<YonderPushButton *> manager_buttons) {
    QJsonObject buttons;
    for(int i=0; i < manager_buttons.length(); i++) {
        buttons.insert(QString("%1").arg(manager_buttons.at(i)->getPosInArray()), QJsonValue(QString("%1").arg(manager_buttons.at(i)->isChecked())));
    }
    return buttons;
}


QJsonObject Webapp::getButtonsStatesForManager(QList<HotkeysPushButton *> manager_buttons) {
    QJsonObject buttons;
    for(int i=0; i < manager_buttons.length(); i++) {
        buttons.insert(QString("%1").arg(manager_buttons.at(i)->getPosInArray()), QJsonValue(QString("%1").arg(manager_buttons.at(i)->isChecked())));
    }
    return buttons;
}

QVariant Webapp::toJsonHelper(QJsonObject buttonlist) {
    QString json = "{";
    QJsonObject::const_iterator i;
    for(i = buttonlist.begin(); i != buttonlist.end(); ++i) {
        json.append(QString("\"%1\": \"%2\",").arg(i.key(), i.value().toString()));
    }
    json.remove(-1, 1);
    json.append("}");
    return json;
}

QString Webapp::routeRoot(Arguments *arg) {
    QSettings settings;

    QFile template_file(":/webservice/templates/main.htm");
    if(!template_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString("");
    }
    QString template_string = QString(template_file.readAll());
    template_string.replace("{{ page_title }}", tr("Yonder Remote Control"));
    template_string.replace("{{ webapp_title }}", tr("Yonder Remote Control"));
    template_string.replace("{{ update_interval }}", settings.value("Settings/webapp_refresh_interval", 10000).toString());
    return template_string;
}


QString Webapp::routeHotkeys(Arguments *arg) {
    if(arg->get("state").toString() == "checked") {
        hotkeys->hotkeys_buttons.at(arg->get("pos").toInt())->setChecked(true);
        return QString("{\"true\"}");
    } else {
        hotkeys->hotkeys_buttons.at(arg->get("pos").toInt())->setChecked(false);
        return QString("{\"false\"}");
    }
}

QString Webapp::routeAtmosphere(Arguments *arg) {
    if(arg->get("state").toString() == "checked") {
        hotkeys->atmosphere_buttons.at(arg->get("pos").toInt())->setChecked(true);
        return QString("{\"true\"}");
    } else {
        hotkeys->atmosphere_buttons.at(arg->get("pos").toInt())->setChecked(false);
        return QString("{\"false\"}");
    }
}

QString Webapp::routeSingleshots(Arguments *arg) {
    if(arg->get("state").toString() == "checked") {
        hotkeys->singleshot_buttons.at(arg->get("pos").toInt())->setChecked(true);
        return QString("{\"true\"}");
    } else {
        hotkeys->singleshot_buttons.at(arg->get("pos").toInt())->setChecked(false);
        return QString("{\"false\"}");
    }
}

QString Webapp::routeSfx(Arguments *arg) {
    if(arg->get("state").toString() == "checked") {
        hotkeys->sfx_buttons.at(arg->get("pos").toInt())->setChecked(true);
        return QString("{\"true\"}");
    } else {
        hotkeys->sfx_buttons.at(arg->get("pos").toInt())->setChecked(false);
        return QString("{\"false\"}");
    }
}

QString Webapp::routeJson(Arguments *arg) {
    if(arg->get("manager").toString() == "hotkeys") {
        return toJsonHelper(getButtonsForManager(hotkeys->hotkeys_buttons)).toString();
    }
    if(arg->get("manager").toString() == "atmosphere") {
        return toJsonHelper(getButtonsForManager(hotkeys->atmosphere_buttons)).toString();
    }
    if(arg->get("manager").toString() == "singleshots") {
        return toJsonHelper(getButtonsForManager(hotkeys->singleshot_buttons)).toString();
    }
    if(arg->get("manager").toString() == "sfx") {
        return toJsonHelper(getButtonsForManager(hotkeys->sfx_buttons)).toString();
    }
    return QString("");
}

QString Webapp::routeJsonState(Arguments *arg) {
    if(arg->get("manager").toString() == "hotkeys") {
        return toJsonHelper(getButtonsStatesForManager(hotkeys->hotkeys_buttons)).toString();
    }
    if(arg->get("manager").toString() == "atmosphere") {
        return toJsonHelper(getButtonsStatesForManager(hotkeys->atmosphere_buttons)).toString();
    }
    if(arg->get("manager").toString() == "singleshots") {
        return toJsonHelper(getButtonsStatesForManager(hotkeys->singleshot_buttons)).toString();
    }
    if(arg->get("manager").toString() == "sfx") {
        return toJsonHelper(getButtonsStatesForManager(hotkeys->sfx_buttons)).toString();
    }
    return QString("");
}


