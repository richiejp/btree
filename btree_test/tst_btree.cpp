#include <QString>
#include <QtTest>
#include "../btree.h"

class Btree_test : public QObject
{
    Q_OBJECT

public:
    Btree_test();

private Q_SLOTS:
    void single();
    void addOne();
    void addMore();
    void addAlotMore();
    void findAlotMore();
};

Btree_test::Btree_test()
{
}

void Btree_test::single() {
    BTree *t = new BTree(3, 100);

    QVERIFY(!t->contains(99));
    QVERIFY(!t->contains(101));
    QVERIFY(t->contains(100));

    t = new BTree(8, 100);

    QVERIFY(!t->contains(99));
    QVERIFY(!t->contains(101));
    QVERIFY(t->contains(100));
}

void Btree_test::addOne() {
    BTree *t = new BTree(6, 100);
    t->add(-100);

    QVERIFY(!t->contains(0));
    QVERIFY(t->contains(-100));
    QVERIFY(t->contains(100));
}

void Btree_test::addMore() {
    BTree *t = new BTree(3, 100);

    t->add(20934);
    t->add(-29384);
    t->add(12345);
    t->add(77777);
    t->add(55555);
    t->add(87873);
    t->add(273847);

    QVERIFY(t->contains(-29384));
    QVERIFY(t->contains(87873));
}

void Btree_test::addAlotMore() {
    BTree *t = new BTree(4, INT_MAX);

    QBENCHMARK_ONCE {
        for(int c = -0x1fffff; c < 0xfffff; c++) {
            t->add(c);
        }
    }
}

void Btree_test::findAlotMore() {
    BTree *t = new BTree(4, INT_MAX);

        for(int c = -0x1fffff; c < 0xfffff; c++) {
            t->add(c);
        }


    QBENCHMARK {
        t->find(1);
        t->find(22);
        t->find(333);
    }
}

QTEST_APPLESS_MAIN(Btree_test)

#include "tst_btree.moc"
