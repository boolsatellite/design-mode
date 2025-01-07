class Font {
  string key;
public:
  Font(const string& key) {

  }
};

class FontFactory {
  map<string , Font*> fontPool;
public:
  Font* GetFont(const string& key) {
    map<string , Font*>::iterator item = fontPool.find(key);
    if(item != fontPool.end()) {
      return footPool[key];
    } else {
      Font* font = new Font(key);
      fontPool[key] = font;
      return font;
    }
    return nullptr;
  }
};