#include "rawconvertor.h"

RawConvertor::RawConvertor()
{


    for (int i = 0; i < INVALID_ASSET; ++i)
        bpStringsList << QString();
}

void RawConvertor::readRawsList(const QList<int>& bpValuesList)
{

    // need to compose those that need composition
    // need to link resulting string to [indexBodypart] string

//    for (const int bp : bpValuesList){

//        // roundness + wide vs narrow
//    }

    bpStringsList[EYES] =
        valueToString(EYES_ROUND_VS_NARROW_1, bpValuesList[EYES_ROUND_VS_NARROW_1]) +
        "_" +
        valueToString(EYES_CLOSE_SET_1,       bpValuesList[EYES_CLOSE_SET_1]);


    bpStringsList[EYEBROWS] =
            valueToString(EYEBROWS_DENSE_1,  bpValuesList[EYEBROWS_DENSE_1]) +
            "_" +
            valueToString(EYEBROWS_LENGTH_1, bpValuesList[EYEBROWS_LENGTH_1]) +
            "_" +
            valueToString(EYEBROWS_HIGH_POSITION_1, bpValuesList[EYEBROWS_HIGH_POSITION_1]);


    bpStringsList[NOSE] =
            valueToString(NOSE_CONVEX,  bpValuesList[NOSE_CONVEX]) +
            "_" +
            valueToString(NOSE_UPTURNED, bpValuesList[NOSE_UPTURNED]);


    bpStringsList[EARS] =
            valueToString(EARS_HANGING_LOBES_1,  bpValuesList[EARS_HANGING_LOBES_1]) +
            "_" +
            valueToString(EARS_HEIGHT_1, bpValuesList[EARS_HEIGHT_1])+
            "_" +
            valueToString(EARS_BROADNESS_1, bpValuesList[EARS_BROADNESS_1]);


    // fabulate lips length
    QStringList randList = {"short", "medium", "long"};
    QString randomLength = randList[(QRandomGenerator::global()->bounded(0,2))];


    bpStringsList[LIPS] =
            valueToString(LIPS_THICKNESS_1,  bpValuesList[LIPS_THICKNESS_1]) +
            "_" +
            randomLength;

}

QString RawConvertor::valueToString(const int bp, const int value)
{

    // assuming value is from eyes (different interval for different body part)


    //NOSE BROADNESS

//  0 - 30          extremely narrow
//  31 - 60         narrow
//  61 - 71         somewhat narrow
//  72 - 128        NONE!
//  129 - 149       somewhat broad
//  150 - 189       broad
//  190 >           extraordinarily broad



    //NOSE LENGTH

//  0 - 30          extremely short
//  31 - 60         short
//  61 - 71         somewhat short
//  72 - 128        NONE!
//  129 - 149       somewhat long
//  150 - 189       quite long
//  190 >           extremely long




    switch (bp){


            // EARS HANGING LOBES

        // 0 - 10           fused lobes
        // 11 - 50          nearly fused lobed
        // 51 - 76          small-lobed
        // 77 - 123         NONE!
        // 124 - 149        free lobed
        // 150 - 189        hanging lobed
        // 190 >            great lobed


        case EARS_HANGING_LOBES_1:
        case EARS_HANGING_LOBES_2:
            if (value <= 10)
                return "fused";

            if (value >= 11 && value <= 50)
                return "fused";

            if (value >= 51 && value <= 76)
                return "fused";

            if (value >= 77 && value <= 123)
                return "small";

            if (value >= 124 && value <= 149)
                return "small";

            if (value >= 150 && value <= 189)
                return "hanging";

            if (value >= 190)
                return "hanging";

            break;



         // EARS BROADNESS


        // 0 - 91           extremely narrow
        // 92 - 94          narrow
        // 95 - 96          somewhat   narrow
        // 97 - 103         NONE!
        // 104 - 105        somewhat   broad
        // 106 - 108        broad
        // 109 >            extraordinary broad


        case EARS_BROADNESS_1:
        case EARS_BROADNESS_2:
            if (value <= 91)
                return "narrow";

            if (value >= 92 && value <= 94)
                return "narrow";

            if (value >= 95 && value <= 96)
                return "normal";

            if (value >= 97 && value <= 103)
                return "normal";

            if (value >= 104 && value <= 105)
                return "normal";

            if (value >= 106 && value <= 108)
                return "broad";

            if (value >= 109)
                return "broad";


            break;




        // EARS HEIGHT


        //  0 - 91           very short
        //  92 - 94          short
        //  95 - 96          somewhat   short
        //  97 - 103         NONE!
        //  104 - 105        somewhat   tall
        //  106 - 108        tall
        //  109 >            extremely broad


        case EARS_HEIGHT_1:
        case EARS_HEIGHT_2:
            if (value <= 91)
                return "short";

            if (value >= 92 && value <= 94)
                return "short";

            if (value >= 95 && value <= 96)
                return "medium";

            if (value >= 97 && value <= 103)
                return "medium";

            if (value >= 104 && value <= 105)
                return "medium";

            if (value >= 106 && value <= 108)
                return "tall";

            if (value >= 109)
                return "tall";

            break;


            // EYES ROUND VS NARROW

            //    ;   0 -  10       slit
            //    ;  11 -  50       narrow
            //    ;  51 -  76       somewhat narrow
            //    ;  77 -  123      ?!? (normal?)
            //    ; 124 -  149      slightly rounded
            //    ; 150 -  189      round
            //    ; 190 >           very round


        case EYES_ROUND_VS_NARROW_1:
        case EYES_ROUND_VS_NARROW_2:
            if (value <= 10)
                return "slit";

            if (value >= 11 && value <= 50)
                return "narrow";

            if (value >= 51 && value <= 76)
                return "somewhatnarrow";

            if (value >= 77 && value <= 123)
                return "slightlyround";

            if (value >= 124 && value <= 149)
                return "round";

            if (value >= 150 && value <= 189)
                return "veryround";

            if (value >= 190)
                 return (QRandomGenerator::global()->bounded(0,1)) ? "extremelyround" : "incrediblyround";

            break;



            // EYES CLOSE SET

            //    ;     very wide set
            //    ;     slightly wide set
            //    ;		wide set

            //    ;     slightly close set
            //    ;		close set
            //    ;     incredibly close set


        case EYES_CLOSE_SET_1:
        case EYES_CLOSE_SET_2:
            if (value <= 10)
                return "wide";

            if (value >= 11 && value <= 50)
                return "wide";

            if (value >= 51 && value <= 76)
                return "normal";

            if (value >= 77 && value <= 123)
                return "normal";

            if (value >= 124 && value <= 149)
                return "close";

            if (value >= 150 && value <= 189)
                return "close";

            if (value >= 190)
                 return "close";

            break;



        //EYEBROW DENSE

        //  0 - 55      extremely sparse
        //  56 - 70     quite sparse
        //  71          slightly sparse
        //  72 - 128    NONE!
        //  129		    slightly dense
        //  130 - 144   quite dense
        //  145 >       extremely dense


        case EYEBROWS_DENSE_1:
        case EYEBROWS_DENSE_2:

        if (value <= 55)
            return "thin";

        if (value >= 56 && value <= 70)
            return "thin";

        if (value == 71)
            return "thin";

        if (value >= 72 && value <= 128)
            return "thick";

        if (value == 129)
            return "thick";

        if (value >= 130 && value <= 144)
            return "bushy";

        if (value >= 145)
             return "bushy";

        break;


        //EYEBROW LENGTH

    //  <= 4        NONE!
    //   5 - 71     somewhat short
    //   72 - 128   NONE!
    //   129        somewhat long
    //   130 - 144  quite long
    //   145 >=     extremely long



        case EYEBROWS_LENGTH_1:
        case EYEBROWS_LENGTH_2:
            if (value <= 4)
                return "short";

            if (value >= 5 && value <= 50)
                return "short";

            if (value >= 51 && value <= 76)
                return "short";

            if (value >= 77 && value <= 123)
                return "medium";

            if (value >= 124 && value <= 149)
                return "medium";

            if (value >= 150 && value <= 189)
                return "long";

            if (value >= 190)
                 return "long";

            break;



        //EYEBROW EYEBROWS HIGH POSITION

        //  0 - 10      extremely low
        //  11 - 50     low
        //  51 - 76     slightly low
        //  77 - 123    NONE!
        //  124 - 149   somewhat high
        //  150 - 189   high
        //  190 >       incredibly high


        case EYEBROWS_HIGH_POSITION_1:
        case EYEBROWS_HIGH_POSITION_2:
            if (value <= 10)
                return "low";

            if (value >= 11 && value <= 50)
                return "low";

            if (value >= 51 && value <= 76)
                return "low";

            if (value >= 77 && value <= 123)
                return "normal";

            if (value >= 124 && value <= 149)
                return "normal";

            if (value >= 150 && value <= 189)
                return "high";

            if (value >= 190)
                 return "high";

            break;


            //NOSE CONVEX

        // 0 - 10           incredibly concave
        // 11 - 50          concave
        // 51 - 76          somewhat concave
        // 77 - 123         NONE!
        //  124 - 149       slightly convex
        //  150 - 189       convex
        //  190 >           very convex


        case NOSE_CONVEX:
            if (value <= 10)
                return "concav";

            if (value >= 11 && value <= 50)
                return "concav";

            if (value >= 51 && value <= 76)
                return "concav";

            if (value >= 77 && value <= 123)
                return "straight";

            if (value >= 124 && value <= 149)
                return "straight";

            if (value >= 150 && value <= 189)
                return "convex";

            if (value >= 190)
                 return "convex";

            break;






            //NOSE UPTURNED

        //  0 - 10          sharply hooked
        //  11 - 50         hooked
        //  51 - 76         slightly hooked
        //  77 - 123        NONE!
        //  124 - 149       slightly upturned
        //  150 - 189       upturned
        //  190 >           incredibly upturned



        case NOSE_UPTURNED:
            if (value <= 10)
                return "hooked";

            if (value >= 11 && value <= 50)
                return "hooked";

            if (value >= 51 && value <= 76)
                return "hooked";

            if (value >= 77 && value <= 123)
                return "normal";

            if (value >= 124 && value <= 149)
                return "normal";

            if (value >= 150 && value <= 189)
                return "upturned";

            if (value >= 190)
                 return "upturned";

            break;



            // LIPS THICKNESS

        //  0 - 55          very thin
        //  56 - 70         thin
        //  71              somewhat thin
        //  72 - 128        NONE!
        //  129 - 149       slightly thick
        //  150 - 189       thick
        //  190 >           very thick


        case LIPS_THICKNESS_1:
        case LIPS_THICKNESS_2:

            if (value <= 55)
                return "thin";

            if (value >= 56 && value <= 70)
                return "thin";

            if (value == 71)
                return "normal";

            if (value >= 72 && value <= 128)
                return "normal";

            if (value >= 129 && value <= 149)
                return "thick";

            if (value >= 150 && value <= 189)
                return "thick";

            if (value >= 190)
                return "thick";

            break;


    }

    // failed to identify bp
    return "";
}

QList<QString> RawConvertor::getRawsTranslated() const
{
    return bpStringsList;
}




















