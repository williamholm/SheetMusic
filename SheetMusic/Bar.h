#pragma once
#include <vector>
#include <string>
#include <array>
enum class ExpressionFlags : unsigned __int64
{
	NO_EXPRESSION	= 0, //can remove this - saving for important spot, maybe decorations?
	FFFF			= 1 << 0,
	FFF				= 1 << 1,
	FF				= 1 << 2,
	F				= 1 << 3,
	MF				= 1 << 4,
	MP				= 1 << 5,
	P				= 1 << 6,
	PP				= 1 << 7,
	PPP				= 1 << 8,
	CRESC			= 1 << 9,
	DESC			= 1 << 10,
	RIT				= 1 << 11,
	A_TEMPO			= 1 << 12,
	INCREASE_TEMPO  = 1 << 13, //need correct name for this
	TENUTO			= 1 << 14,
	STACCATO		= 1 << 15,
	ACCENTED		= 1 << 16,
	TRILL			= 1 << 17,
	MORDENT_UP		= 1 << 18, //need correct names for these
	MORDENT_DOWN	= 1 << 19,
	CRESC_START		= 1 << 20, //can remove CRESC / DESC for more space if needed.
	CRESC_END		= 1 << 21, //Should also end when new volume is specified
	DESC_START		= 1 << 22,
	DESC_END		= 1 << 23,
	SLUR_START		= 1 << 24,
	SLUR_END		= 1 << 25,
	TIE				= 1 << 26,
	HARMONIC		= 1 << 27,
	VIBRATO			= 1 << 28,
	GLISSANDO		= 1 << 29,
	DECORATION		= 1 << 30, //Need to make sure notes with this tag dont have there duration counted.

	//Clefs are here as it makes sense to change per note rather than per bar.
	BASS_CLEF		= size_t(1) << 31, //need to convert to size_t here as int often 4bytes, rather than 8.
	TENOR_CLEF		= size_t(1) << 32,
	TREBLE_CLEF		= size_t(1) << 33,

};

//amount of lines moved up each cleff is set, need to find correct amounts.
constexpr size_t bassClefOffset = 0;
constexpr size_t tenorClefOffset = 0;
constexpr size_t trebleClefOffset = 0;

//the values of each duration is given as the top part of a fraction over 128. Saves using fractions when calculating
//whether a Bar has correct amount of note duration
enum NoteDuration
{
	CORD = 0, //if this note is a cord, only count first note in it.

	STANDARD_128TH	 = 4,
	STANDARD_64TH	 = STANDARD_128TH	 * 2,
	STANDARD_32ND	 = STANDARD_64TH	 * 2,
	STANDARD_16TH	 = STANDARD_32ND	 * 2,
	STANDARD_8TH	 = STANDARD_16TH	 * 2,
	STANDARD_QUATER	 = STANDARD_8TH		 * 2,
	STANDARD_HALF	 = STANDARD_QUATER	 * 2,
	STANDARD_WHOLE	 = STANDARD_HALF	 * 2,

	EXTENDED_128TH	= (STANDARD_128TH	+ 2),
	EXTENDED_64TH	= (STANDARD_64TH	+ STANDARD_128TH),
	EXTENDED_32ND	= (STANDARD_32ND	+ STANDARD_64TH),
	EXTENDED_16TH	= (STANDARD_16TH	+ STANDARD_32ND),
	EXTENDED_8TH	= (STANDARD_8TH		+ STANDARD_16TH),
	EXTENDED_QUATER = (STANDARD_QUATER	+ STANDARD_8TH),
	EXTENDED_HALF	= (STANDARD_HALF	+ STANDARD_QUATER),
	EXTENDED_WHOLE	= (STANDARD_WHOLE	+ STANDARD_HALF),

	DOUBLE_EXTENDED_128TH	= (EXTENDED_128TH	+ 1),
	DOUBLE_EXTENDED_64TH	= (EXTENDED_64TH	+ 2),
	DOUBLE_EXTENDED_32ND	= (EXTENDED_32ND	+ STANDARD_128TH),
	DOUBLE_EXTENDED_16TH	= (EXTENDED_16TH	+ STANDARD_64TH),
	DOUBLE_EXTENDED_8TH		= (EXTENDED_8TH		+ STANDARD_32ND),
	DOUBLE_EXTENDED_QUATER	= (EXTENDED_QUATER	+ STANDARD_16TH),
	DOUBLE_EXTENDED_HALF	= (EXTENDED_HALF	+ STANDARD_8TH),
	DOUBLE_EXTENDED_WHOLE	= (EXTENDED_WHOLE	+ STANDARD_QUATER),

	TEST_FROM_WINTER_COMP 
	= DOUBLE_EXTENDED_QUATER + STANDARD_16TH + EXTENDED_8TH + EXTENDED_8TH + STANDARD_16TH + STANDARD_16TH,

};

//some pitches are technically the same in 12-TET but need to be an option here for display purposes.
enum NoteID
{
	C_FLAT,	C_NATURAL,	C_SHARP,
	D_FLAT,	D_NATURAL,	D_SHARP,
	E_FLAT,	E_NATURAL,	E_SHARP,
	F_FLAT,	F_NATURAL,	F_SHARP,
	G_FLAT,	G_NATURAL,	G_SHARP,
	A_FLAT,	A_NATURAL,	A_SHARP,
	B_FLAT,	B_NATURAL,	B_SHARP,
	REST
};
static constexpr std::array<std::string_view, REST+1> g_note_name_array =
{
	"Cb",	"C",	"C#",
	"Db",	"D",	"D#",
	"Eb",	"E",	"E#",
	"Fb",	"F",	"F#",
	"Gb",	"G",	"G#",
	"Ab",	"A",	"A#",
	"Bb",	"B",	"B#",
	"Rest"
};
//might need to make constexpr array with this to ensure constexpr. check perfomace.
//Can use map if non_constexpr, maybe can make constexpr map instead?
constexpr NoteID noteNameToID(const std::string_view& noteName)
{
	size_t currentValue = C_NATURAL * (noteName.starts_with("C")) + D_NATURAL * (noteName.starts_with("D"))
						+ E_NATURAL * (noteName.starts_with("E")) + F_NATURAL * (noteName.starts_with("F"))
						+ G_NATURAL * (noteName.starts_with("G")) + A_NATURAL * (noteName.starts_with("A"))
						+ B_NATURAL * (noteName.starts_with("B"));
	currentValue += REST * (currentValue == 0); //if value is 0, set it must be rest note,
	currentValue += -(noteName.length() == 2 && noteName.at(1) == 'b') + (noteName.length() == 2 && noteName.at(1) == '#');

	return (NoteID)currentValue;
}

struct Pitch
{
	NoteID noteName;
	size_t octave; //maybe bind octave val < 8 or 9?
	Pitch(NoteID noteName, size_t octave) : noteName(noteName), octave(octave)
	{}
	Pitch() {};
};

struct Note
{
	Pitch pitch;
	NoteDuration duration;
	ExpressionFlags expressions;

	std::string noteToString() const
	{
		//(pitch,duration,expressionFlags)
		//eg (D#3,1,2)
		std::string noteString = "(";
		noteString += g_note_name_array[pitch.noteName].data();
		noteString += "," + std::to_string(pitch.octave) + ","
			+ std::to_string(duration) + "," + std::to_string((int)expressions) +  ")";
		
		return noteString;
	}
	Note(Pitch pitch, NoteDuration duration,  ExpressionFlags expressions = (ExpressionFlags)0):
		duration(duration),	pitch(pitch), expressions(expressions)
	{}
	Note(const std::string& noteString) //this code more unclear than needed, might be worth clarifying later.
	{
		//format is NoteName,Octave,NoteDuration,ExpressionFlags
		//eg		D#,3,128,4									//incase of rest note.
		size_t noteNameEndIndex = !(noteString.at(1) == ',') + 2*(noteString.starts_with("R")); //checks if note is 1 or 2 or 4 characters
		size_t noteDurationStartIndex = noteNameEndIndex + 4; //skip octave and first 2 commas
		size_t noteDurationEndIndex = noteDurationStartIndex + 1 + !(noteString.at(noteDurationStartIndex +2) == ',')
			- 2*(noteString.at(noteDurationStartIndex) == '0'); //incase there is cord.

		pitch = Pitch(noteNameToID(noteString.substr(0, noteNameEndIndex+1).c_str()), noteString.at(noteNameEndIndex+2)-48);
		duration = (NoteDuration)std::stoi(noteString.substr(noteDurationStartIndex, 1 + noteDurationEndIndex- noteDurationStartIndex));
		expressions = (ExpressionFlags)std::stoi(noteString.substr(noteDurationEndIndex+2, noteString.length()- noteDurationEndIndex + 2));
	}
};
struct TimeSignature
{
	size_t mNoOfDivisions; //get proper names for this
	NoteDuration mDivision;
	size_t mValueIn128ths; //number which note durations in a bar need to add up to
	TimeSignature(size_t noOfDivisions, NoteDuration division)
	{ 
		mNoOfDivisions = noOfDivisions;
		mDivision = division;
		mValueIn128ths = division * noOfDivisions;
	}

	//expected format "T mNoOfDivisions mDivision"
	//eg T 04 128
	TimeSignature(const std::string& timeSignatureString)
	{
		if (!timeSignatureString.starts_with("T"))
		{
			std::cout << "Error, incorrect TimeSignature format";
		}
		mNoOfDivisions = std::stoi(timeSignatureString.substr(2, 2));
		mDivision = (NoteDuration)std::stoi(timeSignatureString.substr(5, 3));
		mValueIn128ths = mNoOfDivisions * mDivision;
	}
};
enum KeyID
{
	F_MINOR,
	NO_OF_KEYS
};
//should this be a lookup table?
//what we need is : given note eg. D_SHARP return true or false to whether in key or not.
//should be constexpr?
//this class is not the best, look to change later.
struct Key
{
	//notes in Key must be in order CDEFGAB.
	std::array<NoteID,7> notesInKey; 
	bool isNoteInKey(NoteID note) const
	{
		//works as interger division rounds down and both NoteName and notesInKey are ordered correctly.
		return notesInKey[note / 3] == note;
	}
	constexpr Key(NoteID C, NoteID D, NoteID E, NoteID F, NoteID G, NoteID A, NoteID B)
	{
		//ensures key is in right order -- in later versions might need to add option for keys with less notes.
		if (C / 3 == 0 && D / 3 == 1 && E / 3 == 2 && F / 3 == 3 && G / 3 == 4 && A / 3 == 5 && B / 3 == 6)
		{
			notesInKey = { C,D,E,F,G,A,B };
		}
	}
};
constexpr std::array<Key, NO_OF_KEYS> g_keys =
{
	Key(C_NATURAL,D_NATURAL,E_NATURAL,F_NATURAL,G_NATURAL,A_NATURAL,B_FLAT) //F_MINOR
};
//Does this need key? or should that be put in Sheet Music? - key only needed for display I think.
class Bar
{
public:
	std::vector<Note> mNotes;
	size_t totalNoteLengthInBar() const
	{
		size_t total = 0;
		for (const auto& notes : mNotes)
		{
			total += notes.duration;
		}
		return total;
	}
	std::string barToString() const
	{
		std::string barString = "|";
		for (const auto& note : mNotes)
		{
			barString += note.noteToString();
		}
		barString += "|";
		return barString;
	}	
	Bar() {}
	Bar(const std::string& barString)
	{
		//find ( or |, then create Note using substr
		size_t currentOpenBracketIndex = 2;
		size_t stringLength = barString.length();
		for (size_t currentIndex = 3; currentIndex < stringLength; ++currentIndex)
		{
			if (barString.at(currentIndex) == '(' || barString.at(currentIndex) == '|')
			{
				mNotes.emplace_back(Note(barString.substr(currentOpenBracketIndex, currentIndex - currentOpenBracketIndex - 1)));
				currentOpenBracketIndex = currentIndex + 1;
			}
		}
	}
	~Bar() {}
};