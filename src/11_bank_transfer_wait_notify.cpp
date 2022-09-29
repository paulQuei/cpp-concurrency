#include <condition_variable>
#include <iostream>
#include <mutex>
#include <set>
#include <thread>

using namespace std;

class Account {
public:
  Account(string name, double money): mName(name), mMoney(money) {};

public:
  void changeMoney(double amount) {
    unique_lock lock(mMoneyLock);
    // 这就里明明就是只有不够扣的情况才阻塞
    mConditionVar.wait(lock, [this, amount] {
      return mMoney + amount > 0;
    });
    // 这里比较重要 , 所以这里改变了账户余额 , 如果其他 线程 在这个语句 前后 读余额 会发生什么事呢?
    mMoney += amount;
    mConditionVar.notify_all();
  }

  string getName() {
    return mName;
  }

  double getMoney() {
    return mMoney;
  }

private:
  string mName;
  double mMoney;
  mutex mMoneyLock;
  condition_variable mConditionVar;
};

class Bank {
public:
  void addAccount(Account* account) {
    mAccounts.insert(account);
  }

  void transferMoney(Account* accountA, Account* accountB, double amount) {
    // 先从转出账户把钱扣了 , 如果不够扣出 就阻塞在这里 , 直到有人再转钱过来 , 并且余额大于扣除数 , 才唤醒
    accountA->changeMoney(-amount);
    // 这里就是完全的加钱 , 应该不存在阻塞的情况
    accountB->changeMoney(amount);
  }

  double totalMoney() const {
    double sum = 0;
    for (auto &&a : mAccounts) {
      // 这里个地方 很有问题 读的过程中 getMoney 可能会变啊 ...
      sum += a->getMoney();
    }
    return sum;
  }

private:
  set<Account*> mAccounts;
};

mutex sCoutLock;
void randomTransfer(Bank* bank, Account* accountA, Account* accountB) {
  while(true) {
    double randomMoney = ((double)rand() / RAND_MAX) * 100;
    {
      lock_guard guard(sCoutLock);
      cout << "Try to Transfer " << randomMoney
           << " from " << accountA->getName() << "(" << accountA->getMoney()
           << ") to " << accountB->getName() << "(" << accountB->getMoney()
           << "), Bank totalMoney: " << bank->totalMoney() << endl; // 这里 totalMoney 可能小于 200 也可能大于 200 , 所以我必须加上一些注释来解释这个问题
    }
    // 这里注意到 是先输出到终端 而且终端是临界资源 , 终端使用完成后 , 下面的语言执行 , 也可能阻塞进程 , 下面 进入 transferMoney
    bank->transferMoney(accountA, accountB, randomMoney);
  }
}

int main() {
  Account a("Paul", 100);
  Account b("Moira", 100);

  Bank aBank;
  aBank.addAccount(&a);
  aBank.addAccount(&b);

  thread t1(randomTransfer, &aBank, &a, &b);
  thread t2(randomTransfer, &aBank, &b, &a);

  t1.join();
  t2.join();

  return 0;
}