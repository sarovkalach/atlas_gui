/** @addtogroup Simulator
 * @{*/

/** @file */

#ifndef SIMULATORSTARTER_H
#define SIMULATORSTARTER_H
#include <QProcess>

/*!
 *  \~russian \brief Класс для запуска ПО для расчета динамики и храктеристик активности математических моделей
 *
 */
 
class SimulatorStarter: public QProcess {
public:
    //! \~russian Конструктор класса  \~russian
    SimulatorStarter();
};

#endif // SIMULATORSTARTER_H

/*@}*/
