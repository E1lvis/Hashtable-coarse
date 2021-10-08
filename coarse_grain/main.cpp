#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>

#include "Dictionary.cpp"
#include "MyHashtable.cpp"

//Tokenize a string into individual word, removing punctuation at the
//end of words
std::vector<std::vector<std::string>> tokenizeLyrics(const std::vector<std::string> files) {
  std::vector<std::vector<std::string>> ret;

  for(auto filename : files) {
    //std::cout<<"reading "<<filename<<"\n";
    std::vector<std::string> my_vect;
    std::ifstream in (filename);

    std::string line;

    //For each line
    while (getline(in, line, '\n')) {
      //Skip all like starting with [
      if (line[0] == '[')
        continue;

      std::stringstream ssline (line);
      //For all words
      while (ssline) {
        std::string word;
        ssline >> word;
        if (ssline) {
          //remove punctuation at the end of word
          while (word.length() > 0
                 && std::ispunct(word[word.length() - 1])) {
            word.pop_back();
          }
          my_vect.push_back(word);
        }
      }
    }
    //std::cout<<"read "<<my_vect.size()<<" words\n";
    ret.push_back(my_vect);
  }
  return ret;
}

void doTheThing(std::string& w){
MyHashtable<std::string, int> ht;
Dictionary<std::string, int>& dict = ht;

 int count = dict.get(w);
      ++count;
      dict.set(w, count);
}

int main(int argc, char **argv)
{
  if (argc < 4) {
    std::cerr<<"usage: ./main <sources> <testword> <threshold>"<<std::endl;
    return -1;
  }

  // Parse Command Line
  std::string source = argv[1];
  std::string testWord = argv[2];
  int32_t thresholdCount = std::stoi(argv[3]);

  // Obtain List of Files
  std::vector<std::string> files;
  std::ifstream in (source);
  std::string line;
  while (getline(in, line, '\n')) {
    files.push_back(line);
  }

  // Tokenize Lyrics
  auto wordmap = tokenizeLyrics(files);

  MyHashtable<std::string, int> ht;
  Dictionary<std::string, int>& dict = ht;


auto start =std::chrono::steady_clock::now();
  // write code here
  std::vector<std::thread> mythreads;

for (auto & filecontent: wordmap){
    //std::thread mythread (printMinion, i);
    for (auto & w : filecontent) {
      std::thread mythread (doTheThing, std::ref(w));
      mythreads.push_back(std::move(mythread));
    }
    

  }

 auto stop = std::chrono::steady_clock::now();
  std::chrono::duration<double> time_elapsed = stop-start;

 /*for (auto & filecontent: wordmap) {
    for (auto & w : filecontent) {
      int count = dict.get(w);
      ++count;
      dict.set(w, count);
    }
 }*/








  // Check Hash Table Values 
  /* (you can uncomment, but this must be commented out for tests)
  for (auto it : dict) {
    if (it.second > thresholdCount)
      std::cout << it.first << " " << it.second << std::endl;
  }
  */
std::cerr << time_elapsed.count()<<"\n";
  // Do not touch this, need for test cases
  std::cout << ht.get(testWord) << std::endl;

  return 0;
}
