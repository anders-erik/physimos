#pragma once


#include "lib/str.hh"



enum class NoteType
{
	whole,
	half,
	quarter
};

enum class NoteName
{
	C4,
	D4,
	E4,
	F4,
	G4,
	A4,
	B4,
	C5,
};

struct NoteSerializer
{
	static Str note_name_to_str(NoteName _name)
	{	
		Str str;

		switch (_name)
		{
			case NoteName::C4: str = "C4";	break;
			case NoteName::D4: str = "D4";	break;
			case NoteName::E4: str = "E4";	break;
			case NoteName::F4: str = "F4";	break;
			case NoteName::G4: str = "G4";	break;
			case NoteName::A4: str = "A4";	break;
			case NoteName::B4: str = "B4";	break;
			case NoteName::C5: str = "C5";	break;
			
			default:
				break;
		}

		return str;
	}

	static Str note_type_to_str(NoteType _type)
	{	
		Str str;

		switch (_type)
		{
			case NoteType::quarter: str = "quarter";	break;
			case NoteType::half: 	str = "half";		break;
			case NoteType::whole: 	str = "whole";		break;
			
			default:
				break;
		}

		return str;
	}
};

struct Note 
{
	NoteName name;
	NoteType type;

	bool operator==(Note b)
	{
		if(this->name == b.name && this->type == b.type)
			return true;
		
		return false;
	}
};


// const Note A4 { "A4", 440.0, NoteType::quarter};
// const Note A4 { NoteName::A4, NoteType::quarter};