#ifndef PERLIN_H
#define PERLIN_H

class Perlin
{
  public:
    static float getPerlinValue(float x, float y);

  private:
    static noise::module::Perlin perlin;
};

#endif // !PERLIN_H