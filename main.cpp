#include "locale.h"
#include "wchar.h"
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <windows.h>
using namespace std;
using namespace filesystem;
int main() {
  /*Use CreateMuteA function to avoid multi-process*/
  HANDLE If_mute_process = CreateMutexA(NULL, FALSE, "regex_qqmusic_renamer");
  if (GetLastError() == ERROR_ALREADY_EXISTS) {
    CloseHandle(If_mute_process);
    If_mute_process = NULL;
    exit(0);
  }

  /*Set local of the rigion to avoid error caused by wrong coding*/
  setlocale(LC_ALL, "en_US.utf8");
  /*Init file path*/
  path file_dir = L"D:\\Desktop\\Media\\Music";

  if (!exists(file_dir)) {
    exit(1);
  }

  /*Start regex*/
  wsmatch regex_result_group;
  wregex reg(L"(.*\\\\)(.*) - (.*)( \\[.*\\])(\\.)(ogg|mp3|flac|wav)");
  /*Start to enum the dir*/
  directory_iterator list(file_dir);
  for (auto &it : list) {
    wstring temp = it.path().wstring();
    /*if path could match the regex, set the new_name*/
    if (regex_search(temp, regex_result_group, reg)) {
      wstring result = regex_result_group[1].str() +
                       regex_result_group[3].str() + L"." +
                       regex_result_group[6].str();
      wstring original_name = regex_result_group[0].str();
      wstring brief_name = regex_result_group[3].str();
      /*Try to rename*/
      if (0 != _wrename(original_name.c_str(), result.c_str())) {
        wcout << "Error! " << brief_name << endl;
      } else {
        wcout << "Successfully! " << brief_name << endl << endl;
      }
    }
  }
  return 0;
}
