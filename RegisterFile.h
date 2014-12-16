#ifndef __REGISTER_FILE_H__
#define __REGISTER_FILE_H__

#define REGISTER_FILE_SIZE 128

#include <map>

class RegisterFile
{
  struct Register
  {
    int name;
    bool ready;
    Register() {name = -1; ready=true;}
  };
  public:
  // update register ready status if the name matches
  void update(int name)
  {
    if(name == -1) return;
    std::map<int,int>::iterator iter = map.find(name);
    if(iter == map.end()) return;
    if(file[iter->second].name == name)
    {
      file[iter->second] = Register();
    }
    map.erase(iter);
  }
  // renaming register and setting ready status to false
  int rename(int reg, int name)
  {
    if(reg == -1|| name == -1) return -1;
    file[reg].name = name;
    file[reg].ready = false;
    map.insert(std::pair<int,int>(name,reg));
    return name;
  }
  const Register & operator[] (int reg)
  {
    if(reg == -1)
      return def;
    return file[reg];
  }
  private:
  const Register def;
  Register file[REGISTER_FILE_SIZE];
  std::map<int,int> map;
};

#endif //__REGISTER_FILE_H__
