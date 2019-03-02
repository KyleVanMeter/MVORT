#include <iostream>
#include <string>
#include <map>

#include "Society.h"

bool City::IsBig() const {
    if (population_ > 100000) {
        return true;
    }

    return false;
}

void City::Grow(int const num_people) {
    if (num_people >= 0) {
        int prev = population_;
        population_ += num_people;
        percent_change_ = (num_people) / prev;
    }
}

void City::Shrink(int const num_people) {
    if (num_people >= 0) {
        population_ -= num_people;
    }
}

void Society::GrowCities() {
    City * biggest = cities_.at(1);
    int bigPop = biggest->get_population();

    for(int i = 2; i < 5; i++) {
      if(cities_.at(i)->get_population() > bigPop) {
        biggest = cities_.at(i);
      }
    }

    for (int i = 1; i <= 4; i++) {
        City * c = cities_.at(i);
        if (!c->IsBig()){
            int prev = c->get_population();
            double ratio = (1.0 * c->get_population()) / biggest->get_population();
            c->Grow((int) ratio * .3 * c->get_population());
            c->Shrink((int) c->get_population() * .1);
            double percent_change = (1.0 * c->get_population() - prev) / prev;
            c->set_percent_change(percent_change);
        } else {
            int prev_small = c->get_population();
            double ratio = (1.0 * c->get_population()) / biggest->get_population();
            c->Grow((int) ratio * c->get_population());
            c->Shrink((int) c->get_population() * .1);
            double percent_change_small = (1.0 * c->get_population() - prev_small) / prev_small;
            c->set_percent_change(percent_change_small);
        }
    }
}

std::ostream& operator<<(std::ostream &os, const City &c) {
    os << c.name_ << ": " << c.population_  << " (" << c.percent_change_ << "% change)";
    return os;
}

std::ostream& operator<<(std::ostream &os, const Society &s) {
    for(int i = 1; i < 5; i++) {
      os << *(s.cities_.at(i)) << std::endl;
    }
    return os;
}
