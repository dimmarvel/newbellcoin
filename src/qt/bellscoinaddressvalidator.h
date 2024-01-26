// Copyright (c) 2011-2020 The bellscoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BELLSCOIN_QT_BELLSCOINADDRESSVALIDATOR_H
#define BELLSCOIN_QT_BELLSCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BellscoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BellscoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

/** bellscoin address widget validator, checks for a valid bellscoin address.
 */
class BellscoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BellscoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const override;
};

#endif // BELLSCOIN_QT_BELLSCOINADDRESSVALIDATOR_H
