#ifndef __POTION_H__
#define __POTION_H__

#include "Item.h"

class Potion: public Item {
    protected:
        const unsigned int recovery_points;

    public:
        Potion(const unsigned int id, 
               const unsigned int price,
               const unsigned int recovery_points);
        virtual ~Potion();

        Potion(const Potion&) = delete;
        Potion& operator=(const Potion&) = delete;
        Potion(Potion&&) = delete;
        Potion& operator=(Potion&&) = delete;

        virtual void equip(Character& character) = 0;
};

class HealthPotion: public Potion {
    public:
        HealthPotion(const unsigned int id, 
                     const unsigned int price,
                     const unsigned int recovery_points);
        ~HealthPotion();

        virtual void equip(Character& character) override;
};

class ManaPotion: public Potion {
    public:
        ManaPotion(const unsigned int id,
                   const unsigned int price,
                   const unsigned int recovery_points);
        ~ManaPotion();

        virtual void equip(Character& character) override;
};

#endif
