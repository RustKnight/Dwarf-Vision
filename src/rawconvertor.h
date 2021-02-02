#ifndef RAWCONVERTOR_H
#define RAWCONVERTOR_H


#include "Ticket.h"
#include "QRandomGenerator"

//ears
//	- Splayed out: 14, 15
//	- Hanging lobes: 16, 17
//	- Broadness: 18, 19
//	- Height: 20, 21

//cheeks
//	- NONE!

//lips
//	- Thickness: 8, 9

//beard
//	- INCOMING

//moustache
//	- INCOMING

//eyes
//	- Close set: 0, 1
//	- Deep set: 2, 3
//	- Round vs narrow: 4, 5
//	- Large iris: 6, 7

//nose
//	- Broadness: 10
//	- Length: 11
//	- Upturned: 12
//	- Convex: 13

//eyebrows
//	- Length: 42, 43
//	- Dense: 44, 45
//	- High position: 46, 47

//hair
//	- INCOMING


// strings will compose in same order as actual PNG strigs - will be later found via (for each str in strList, if str.contains(rawString))


// maybe just get rid of duplicates? EYES_CLOSE_SET_1 and EYES_CLOSE_SET_2 ?

enum rawIndex { EARS_SPLAYED_OUT_1 = 14, EARS_SPLAYED_OUT_2 = 15,
                EARS_HANGING_LOBES_1 = 16, EARS_HANGING_LOBES_2 = 17,
                EARS_BROADNESS_1 = 18, EARS_BROADNESS_2 = 19,
                EARS_HEIGHT_1 = 20, EARS_HEIGHT_2 = 21,

                EYES_CLOSE_SET_1 = 0, EYES_CLOSE_SET_2 = 1,
                EYES_ROUND_VS_NARROW_1 = 4, EYES_ROUND_VS_NARROW_2 = 5,

                EYEBROWS_LENGTH_1 = 42, EYEBROWS_LENGTH_2 = 43,
                EYEBROWS_DENSE_1 = 44, EYEBROWS_DENSE_2 = 45,
                EYEBROWS_HIGH_POSITION_1 = 46, EYEBROWS_HIGH_POSITION_2 = 47,

                NOSE_BROADNESS = 10,
                NOSE_LENGTH = 11,
                NOSE_UPTURNED = 12,
                NOSE_CONVEX = 13,

                LIPS_THICKNESS_1 = 8, LIPS_THICKNESS_2 = 9,

                // A VERY ROUGH ASSUMPTION THAT THIS INDEX CONTAINS CURLY VALUE
                CURLYNESS_MALE      = 56, // index varies for make/female
                CURLYNESS_FEMALE    = 52};
                // no cheeks!



class RawConvertor
{
public:
    RawConvertor();

    void readRawsList(const QList<int>& bpValuesList);
    QString valueToString (const int bp, const int value);
    QList<QString> getRawsTranslated() const;


private:

    QList<QString> bpStringsList;
};


































#endif // RAWCONVERTOR_H
