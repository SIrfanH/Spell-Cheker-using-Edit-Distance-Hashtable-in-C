# Spell-Cheker-using-Edit-Distance-Hashtable-in-C
A Spell checker program that recommends right way of spelling words to the user. The program first reads a text file that contains right spelling of words and creates a Dictionary (Hash Table) of them. After creating the dictionary when a user writes a sentence the program checks the dictionary for every words in the sentence and if there is a spelling error then the program recommends the right way of spelling it. This program uses Hash Table for the dictionary and an Optimized version of Levenshtein Edit Distance to check the spelling of the words.<br/>

The program Features:<br/>
## Creating The Dictionary
The dictionary hash table is created from words in smallDictionary.txt file. Two hash tables are used in this program. The first is for the dictionary to hold the correct spelling of the words and the second hash table is used to store history of incorrect spellings of a word. Since there are lots of words that people spell the same way but incorrectly, creating an incorrect word spellings hash table will be saving time by not always using edit distance but to check the table. <br/>

Open Addressing technique is used for creating the hash tables and Double hashing method is used to solve the collision problem of the tables.<br/>
## Searching for Words in the Dictionary
After the creation of the dictionary the user may provide a sentence or a passage for the program. Firstly the program checks every word in the passage if they exist in the dictionary. If a word exists in the dictionary then it is considered to be spelled correctly by the user. If a word does not exist in the dictionary then the program assumes that it is not spelled correctly and checks the incorrectly spelled words hash table if the word is in it or not. Since the second hash table holds history of incorrectly spelled words and their correct spelling the program immediately recommends the right spelling of the word to the user.<br/>
## Optimized Levenstein Edit Distance
The levenshtein edit distance is a method for measuring the difference between two sequences. The sequences can be strings or sequence of bits or DNA and RNA sequences, this technique is useful in many disciplines. It determines the difference between two sequences by minimum number of edits (insertion, deletion, and substitution) required to apply on one sequence for it to become like the second sequence. <br/>

The algorithm for applying levenshtein edit distance used in this program uses the dynamic programming convention. For this program the optimized levenshtein edit distance algorithm determines a threshold value K for the number of differences between two sequences. If the number of difference passes K then the program exits the edit distance functions.<br/>
## Recommending Correct Spelling
When the incorrectly spelled word was not in the second hash table then the program uses the levenshtein edit distance function to calculate the differences between all words in the dictionary and the incorrectly spelled word. The word with the least difference is recommended to the user as the correct spelling of that word. After the user selected the correct spelling of the word the the program adds the incorrectly spelled word and its correct spelling to the second hash table to use in the future spell checking.<br/>
<br/>
<p align='center'>
  <img src='https://user-images.githubusercontent.com/49107892/155859997-890f985e-4b0a-473c-91c1-07c8f0766286.png' width='800'>
</p>
