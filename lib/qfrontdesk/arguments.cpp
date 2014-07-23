#include "arguments.h"

Arguments::Arguments(QObject *parent) :
    QObject(parent)
{
    arguments = new QList<args>;
}


QVariant Arguments::get(QString var) {
    for(int i=0; i<arguments->length(); i++) {
        args arg = arguments->at(i);
        if(arg.variable == var) {
            return arg.value;
        }
    }
    return QVariant();
}


QVariant Arguments::at(int pos) {
    args arg = arguments->at(pos);
    return arg.value;
}


void Arguments::set(QString var, QVariant val) {
    args argument;
    argument.variable = var;
    argument.value = val;
    arguments->append(argument);
}


int Arguments::length() {
    return arguments->length();
}

