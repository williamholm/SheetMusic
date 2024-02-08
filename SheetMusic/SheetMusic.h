#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include "Bar.h"
/*
Expected file format:
1: Title
2: Composer
3: Playing Notes? (advanced add later maybe as a vector of pairs of strings and BarNumbers)
(4-6 can be in any order) and have same format for changes, X is a digit that if not present is 0.
4: Starting TimeSignature as	T XX XXX
5: Starting Key as				K XX
6: Starting Tempo				t XXX
7-X: Bars, Tempo, Key and TimeSignature changes.
Bars have Format |(NoteName,NoteDuration,ExpressionFlags)...|
*/


//Either every change on first,2nd and 3rd line, or as they come?
//to do as they come you would need to write something like
/* what if multiple changes?
* if first character is |, do normal bar;
* elseif first character is K, do Key change;
* elseif first character is t, do tempo change;
* elseif first character is T, do TimeSignature change;
*/
class SheetMusic
{
	using BarNumber = size_t; //BarNumber always stored as +1 of actual index to match bar number in actual sheet music

	std::string mFilePath;
	std::string mTitle; //need to cap characters.
	std::string mComposer;
	std::vector<Bar> mBars;
	//if vector is ordered by BarNumber we can: each bar check if next change = current bar - ask dad if any of these changes happen mid bar
	std::vector<std::pair<KeyID, BarNumber>> mKeyChanges;
	std::vector<std::pair<TimeSignature, BarNumber>> mTimeSignatureChanges;
	std::vector<std::pair<size_t, BarNumber>> mTempoChanges;

	//just need to check every bar if it is in any map.
	//std::unordered_map<BarNumber, Key> mKeyChanges;
	//std::unordered_map<BarNumber, TimeSignature> mTimeSignatureChanges;
	//std::unordered_map<BarNumber, int> mTempoChanges;


public:
	void insertBar(BarNumber barNumber, Bar&& bar)
	{
		//check make sure not to have bars are not skipped.
		//need to have + 1 as bars are not 0-indexed in music.
		if (barNumber > mBars.size()+1)
		{
			std::cout << "\nError, bar "  << mBars.size() << " must Exist for bar " << barNumber << " to exist";
			return;
		}
		else if (barNumber == mBars.size()+1)
		{
			mBars.emplace_back(bar);
		}
		else
		{
			mBars[barNumber - 1] = bar;
		}
	}
	void printMusic()
	{
		for (const auto& bars : mBars)
		{
			std::cout << bars.barToString() << "\n";
		}
	}
	void saveMusic()
	{
		std::ofstream stream;
		stream.open(mFilePath,std::ios::out);
		for (const auto& bars : mBars)
		{
			stream << bars.barToString();
		}
		stream.close();
	}
	void readMusic()
	{
		std::ifstream stream;
		stream.open(mFilePath, std::ios::in);
		BarNumber barNumber = 1;
		if (stream)
		{
			//get title
			//get composer
			//read additional notes? - save for later
			std::string barString;
			while (!stream.eof())
			{
				std::getline(stream, barString);
				if (barString.starts_with("|"))
				{
					mBars.emplace_back(Bar(barString));
					++barNumber;
				}
				else if (barString.starts_with("K"))
				{
					KeyID nextKey = (KeyID)std::stoi(barString.substr(2, 2));
					mKeyChanges.emplace_back(std::pair<KeyID, BarNumber>(nextKey, barNumber));
				}
				else if (barString.starts_with("t"))
				{
					size_t nextTempo = std::stoi(barString.substr(2, 3));
					mTempoChanges.emplace_back(std::pair<size_t, BarNumber>(nextTempo, barNumber));
				}
				else if (barString.starts_with("T"))
				{
					mTimeSignatureChanges.emplace_back(std::pair<TimeSignature, BarNumber>(barString,barNumber));
				}
			}
		}
		else
		{
			std::cout << "\nError, file " << mFilePath << " could not be opened";
		}
		stream.close();
	}
	SheetMusic(std::string filePath) : mFilePath(filePath)
	{

	}
};