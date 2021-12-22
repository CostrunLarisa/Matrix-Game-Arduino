class Player {
  private: String name;
  private: long long score;
  public: Player() {
      name = "";
      score = 0;
    }
  public: Player(String nameP, int scoreP) {
      name = nameP;
      score = scoreP;
    }
  public: int getScore() {
      return score;
    }
  public: String getName() {
      return name;
    }
  public: void setScore (int scoreP) {
      score = scoreP;
    }
  public: void setName (String nameP) {
      name = nameP;
    }
};