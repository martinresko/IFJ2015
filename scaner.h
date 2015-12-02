/**
 * Predmet: IFJ / IAL
 * Subor:     scaner.h
 *            Implementacia lexikálnej analýzy
 * Projekt:   Implementace interpretu imperativního jazyka IFJ15
 *            tým 21, varianta a/3/I
 * Autori:    <xhlava42@stud.fit.vutbr.cz>, Dominik Hlaváč Ďurán
 *            <xdurco00@stud.fit.vutbr.cz>, Marián Ďurčo
 *            <xdomon00@stud.fit.vutbr.cz>, Dávid Domonkoš
 *            <xcerna06@stud.fit.vutbr.cz>, Peter Čerňanský
 *            <xbaric01@stud.fit.vutbr.cz>, Filip Barič
 */
/* States of the FSM */
typedef enum{
	sStart,				// 00 - Start
	sIdent,				// 01 - Identifikator
	sInteger,			// 02 - Celociselny literal
	sDouble,			// 03 - Desatinny literal
	sIsExpo,			// 04 - Zaciatok exponentu
	sExpo, 				// 05 - Literal s exponentom
	sString,			// 06 - Retazcovy literal
	sNull,				// 07 - Stav na odstranenie prebytocnych nul
	sAssign,			// 08 - =
	sEqual,				// 09 - ==
	sDivide,			// 10 - /
	sLComment,			// 11 - //
	sBComment,			// 12 - /*
	sBlockEnd,			// 13 - */
	sExclam,			// 14 - !
	sNotEq,				// 15 - !=
	sGreater,			// 16 - >
	sLess,				// 17 - <
	sGrorEq,			// 18 - >=
	sLeorEq,			// 19 - <=
	sSemicolon,			// 20 - ;
	sLParenth,			// 21 - (
	sRParenth,			// 22 - )
	sLSetPar,			// 23 - {
	sRSetPar,			// 24 - }
	sPlus,				// 25 - +
	sMinus,				// 26 - -
	sMult,				// 27 - *
	sEndofFile,			// 28 - End of File
	sComma,				// 29 - ,
	sCout,				// 30 - <<
	sCin,				// 31 - >>
	sKeyWord,			// 32 - Klucove slova
	sResWord,			// 33 - Rezervovane slova
	sError,				// 34 - Lexikalna chyba
	sEnd, 				// 35 - koncovy stav
	sIsDouble,			// 36 - Zaciatok Double
	sIsExpo2,			// 37 - Pokracovanie exponentu
	sEscSeq,			// 38 - Escape sekvencie
	sEscHex,			// 39 - Hexa zapisovanie v Esc sekvencii
	sEscHex2			// 40 - Hexa zapisovanie 2

} tState;

/* Struct for token */
typedef struct{
	int id;
	char *attribute;
} tToken;

/* Globalna premenna */
extern tToken token;

/* Lexikalna chyba */
ERROR_CODE scaner_error;

/* Deklaracia funkcie get_Token */
tToken get_Token();