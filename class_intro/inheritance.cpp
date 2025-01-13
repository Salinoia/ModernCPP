#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <typeinfo>

// 定义元素效果策略接口
class ElementEffect {
public:
    virtual void applyEffect() const = 0;
    virtual ~ElementEffect() {}
};

// 具体元素效果
class FireEffect : public ElementEffect {
public:
    void applyEffect() const override {
        std::cout << "Applying Fire effect!" << std::endl;
    }
};

class IceEffect : public ElementEffect {
public:
    void applyEffect() const override {
        std::cout << "Applying Ice effect!" << std::endl;
    }
};

class LightningEffect : public ElementEffect {
public:
    void applyEffect() const override {
        std::cout << "Applying Lightning effect!" << std::endl;
    }
};

class WindEffect : public ElementEffect {
public:
    void applyEffect() const override {
        std::cout << "Applying Wind effect!" << std::endl;
    }
};

class EarthEffect : public ElementEffect {
public:
    void applyEffect() const override {
        std::cout << "Applying Earth effect!" << std::endl;
    }
};

// 基类 Weapon
class Weapon {
private:
    std::string name;
    double damage;
protected:
    std::unique_ptr<ElementEffect> elementEffect;
public:
    // Modify constructor to accept unique_ptr and transfer ownership
    Weapon(const std::string& weaponName, double dmg, std::unique_ptr<ElementEffect> elemEffect)
        : name(weaponName), damage(dmg), elementEffect(std::move(elemEffect)) {}

    virtual ~Weapon() {}

    virtual void attack() const = 0;

    std::string getName() const { return name; }
    double getDamage() const { return damage; }
};

// 派生类 Bow
class Bow : public Weapon {
private:
    int range; // 弓箭的射程
public:
    Bow(const std::string& bowName, double dmg, std::unique_ptr<ElementEffect> elemEffect, int rng)
        : Weapon(bowName, dmg, std::move(elemEffect)), range(rng) {}

    void attack() const override {
        std::cout << "Shooting an arrow with " << getDamage() << " damage. ";
        elementEffect->applyEffect();// 基类指针调取子类函数，实现动态多态绑定
        std::cout << " Range: " << range << " meters." << std::endl;
    }

    int getRange() const { return range; }
};

// 派生类 Spear
class Spear : public Weapon {
private:
    double thrustSpeed; // 长枪的刺击速度
public:
    Spear(const std::string& spearName, double dmg, std::unique_ptr<ElementEffect> elemEffect, double speed)
        : Weapon(spearName, dmg, std::move(elemEffect)), thrustSpeed(speed) {}// 子类的构造函数必须先调用父类的构造函数

    void attack() const override {
        std::cout << "Thrusting with a spear dealing " << getDamage() << " damage. ";
        elementEffect->applyEffect();
        std::cout << " Thrust speed: " << thrustSpeed << " m/s." << std::endl;
    }

    double getThrustSpeed() const { return thrustSpeed; }
};

// Test function
void testWeaponsOptimized() {
    // Create weapons by transferring ownership of ElementEffect
    std::unique_ptr<Weapon> weapon1 = std::make_unique<Bow>("Longbow", 50.0, std::make_unique<WindEffect>(), 150);
    std::unique_ptr<Weapon> weapon2 = std::make_unique<Spear>("Fire Spear", 75.0, std::make_unique<FireEffect>(), 30.5);
    std::unique_ptr<Weapon> weapon3 = std::make_unique<Bow>("Ice Bow", 60.0, std::make_unique<IceEffect>(), 120);
    std::unique_ptr<Weapon> weapon4 = std::make_unique<Spear>("Earth Spear", 80.0, std::make_unique<EarthEffect>(), 28.0);

    // Store in a container of unique_ptrs
    std::vector<std::unique_ptr<Weapon>> weapons;
    weapons.push_back(std::move(weapon1));
    weapons.push_back(std::move(weapon2));
    weapons.push_back(std::move(weapon3));
    weapons.push_back(std::move(weapon4));

    // Iterate and perform attacks
    for(const auto& weapon : weapons) {
        weapon->attack();
    }

    // Example type conversion
    std::unique_ptr<Weapon> base = std::make_unique<Spear>("Lightning Spear", 85.0, std::make_unique<LightningEffect>(), 32.0);
    base->attack();

    std::cout << std::string(typeid(*base).name()) << std::endl;

    // Attempt to cast to Spear
    Spear* spear = dynamic_cast<Spear*>(base.get());
    if(spear) {
        std::cout << "Successfully casted to Spear: " << spear->getName() << std::endl;
    } else {
        std::cout << "Failed to cast to Spear." << std::endl;
    }

    // No need to manually delete; unique_ptr handles it
}

int main() {
    testWeaponsOptimized();
    return 0;
}


#include <iostream>

class Attack {
public:
    Attack() { std::cout << "Attack constructor" << std::endl; }
    void display() { std::cout << "Class Attack" << std::endl; }
};

class PhysicalAttack : virtual public Attack {
public:
    PhysicalAttack() { std::cout << "PhysicalAttack constructor" << std::endl; }
};

class MagicAttack : virtual public Attack {
public:
    MagicAttack() { std::cout << "MagicAttack constructor" << std::endl; }
};

class Hit : public PhysicalAttack, public MagicAttack {
public:
    Hit() { std::cout << "Hit constructor" << std::endl; }
};

int main() {
    Hit obj;
    obj.display(); // 调用 Attack::display，不会有二义性
    return 0;
}