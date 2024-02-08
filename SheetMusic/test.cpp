#include <iostream>
#include "SheetMusic.h"

int main()
{
   //test to ensure correct conversion of noteNames to NoteIDs
   for (int i = 0; i <= REST; ++i)
   {
       if (noteNameToID(g_note_name_array[i]) != i)
       {
           std::cout << "\n\nError: " << g_note_name_array[i] << " incorrectly converted";
           return -1;
       }
   }
    std::cout << "\n\n====Testing using strings to create Note====\n\n";
   std::vector<Note> testNotes;
   testNotes.emplace_back(Note(std::string("D#,3,128,152352")));
   testNotes.emplace_back(Note(std::string("C,4,64,0")));
   testNotes.emplace_back(Note(std::string("Ab,3,64,1241")));
   testNotes.emplace_back(Note(std::string("Rest,0,64,456")));
   testNotes.emplace_back(Note(std::string("Rest,0,128,0")));
   testNotes.emplace_back(Note(std::string("E#,6,0,0")));
   testNotes.emplace_back(Note(std::string("Bb,4,0,0")));
   testNotes.emplace_back(Note(std::string("A,3,256,0")));

   for (const auto& testNote : testNotes)
   {
       std::cout << testNote.noteToString() << "\n";
   }

   std::cout << "\n\n====Testing using strings to create Bar====\n\n";
   std::vector<Bar> testBars;
   testBars.emplace_back(Bar("|(D,3,128,0)(F,3,128,0)(Bb,3,256,0)|"));
   testBars.emplace_back(Bar("|(F,3,128,0)(F,3,64,0)(E,3,64,0)(C,3,128,0)(A,3,128,0)|"));
   testBars.emplace_back(Bar("|(D,3,128,0)(F,3,128,0)(E,3,256,0)|"));

   for (const auto& bars : testBars)
   {
       std::cout << bars.barToString() << "\n";
   }

   std::cout << "\n\n====Testing SheetMusic====\n\n";
   SheetMusic testSheetMusic("TestMusic/dadMusic1.txt"); 
   testSheetMusic.readMusic();
   testSheetMusic.printMusic();

   //testSheetMusic.saveMusic();
}
