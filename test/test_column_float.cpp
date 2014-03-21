#include "testsettings.hpp"
#ifdef TEST_COLUMN_FLOAT

#include <iostream>

#include <tightdb/column_basic.hpp>

#include "util/unit_test.hpp"
#include "util/test_only.hpp"

using namespace tightdb;


namespace {

template<class T, size_t N> inline size_t size_of_array(T(&)[N])
{
    return N;
}

// Article about comparing floats:
// http://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/

float float_val[] = {
    0.0f,
    1.0f,
    2.12345f,
    12345.12f,
    -12345.12f
};
const size_t float_val_len = size_of_array(float_val);

double double_val[] = {
    0.0,
    1.0,
    2.12345,
    12345.12,
    -12345.12
};
const size_t double_val_len = size_of_array(double_val);

} // anonymous namespace


template <class C>
void BasicColumn_IsEmpty()
{
    C c;
    CHECK(c.is_empty());
    CHECK_EQUAL(0U, c.size());
    c.destroy();
}
TEST(ColumnFloat_IsEmpty) { BasicColumn_IsEmpty<ColumnFloat>(); }
TEST(ColumnDouble_IsEmpty){ BasicColumn_IsEmpty<ColumnDouble>(); }


template <class C, typename T>
void BasicColumn_AddGet(T val[], size_t valLen)
{
    C c;
    for (size_t i=0; i<valLen; ++i) {
        c.add(val[i]);

        CHECK_EQUAL(i+1, c.size());

        for (size_t j=0; j<i; ++j) {
            CHECK_EQUAL(val[j], c.get(j));
        }
    }

    c.destroy();
}
TEST(ColumnFloat_AddGet) { BasicColumn_AddGet<ColumnFloat, float>(float_val, float_val_len); }
TEST(ColumnDouble_AddGet){ BasicColumn_AddGet<ColumnDouble, double>(double_val, double_val_len); }


template <class C, typename T>
void BasicColumn_Clear()
{
    C c;
    CHECK(c.is_empty());

    for (size_t i=0; i<100; ++i)
        c.add();
    CHECK(!c.is_empty());

    c.clear();
    CHECK(c.is_empty());

    c.destroy();
}
TEST(ColumnFloat_Clear) { BasicColumn_Clear<ColumnFloat, float>(); }
TEST(ColumnDouble_Clear){ BasicColumn_Clear<ColumnDouble, double>(); }


template <class C, typename T>
void BasicColumn_Set(T val[], size_t valLen)
{
    C c;
    for (size_t i=0; i<valLen; ++i)
        c.add(val[i]);
    CHECK_EQUAL(valLen, c.size());

    T v0 = T(1.6);
    T v3 = T(-987.23);
    c.set(0, v0);
    CHECK_EQUAL(v0, c.get(0));
    c.set(3, v3);
    CHECK_EQUAL(v3, c.get(3));

    CHECK_EQUAL(val[1], c.get(1));
    CHECK_EQUAL(val[2], c.get(2));
    CHECK_EQUAL(val[4], c.get(4));

    c.destroy();
}
TEST(ColumnFloat_Set) { BasicColumn_Set<ColumnFloat, float>(float_val, float_val_len); }
TEST(ColumnDouble_Set){ BasicColumn_Set<ColumnDouble, double>(double_val, double_val_len); }


template <class C, typename T>
void BasicColumn_Insert(T val[], size_t valLen)
{
    (void)valLen;

    C c;

    // Insert in empty column
    c.insert(0, val[0]);
    CHECK_EQUAL(val[0], c.get(0));
    CHECK_EQUAL(1, c.size());

    // Insert in top
    c.insert(0, val[1]);
    CHECK_EQUAL(val[1], c.get(0));
    CHECK_EQUAL(val[0], c.get(1));
    CHECK_EQUAL(2, c.size());

    // Insert in middle
    c.insert(1, val[2]);
    CHECK_EQUAL(val[1], c.get(0));
    CHECK_EQUAL(val[2], c.get(1));
    CHECK_EQUAL(val[0], c.get(2));
    CHECK_EQUAL(3, c.size());

    // Insert at buttom
    c.insert(3, val[3]);
    CHECK_EQUAL(val[1], c.get(0));
    CHECK_EQUAL(val[2], c.get(1));
    CHECK_EQUAL(val[0], c.get(2));
    CHECK_EQUAL(val[3], c.get(3));
    CHECK_EQUAL(4, c.size());

    // Insert at top
    c.insert(0, val[4]);
    CHECK_EQUAL(val[4], c.get(0));
    CHECK_EQUAL(val[1], c.get(1));
    CHECK_EQUAL(val[2], c.get(2));
    CHECK_EQUAL(val[0], c.get(3));
    CHECK_EQUAL(val[3], c.get(4));
    CHECK_EQUAL(5, c.size());

    c.destroy();
}
TEST(ColumnFloat_Insert) { BasicColumn_Insert<ColumnFloat, float>(float_val, float_val_len); }
TEST(ColumnDouble_Insert){ BasicColumn_Insert<ColumnDouble, double>(double_val, double_val_len); }


template <class C, typename T>
void BasicColumn_Aggregates(T val[], size_t valLen)
{
    (void)valLen;
    (void)val;

    C c;

//    double sum = c.sum();
//    CHECK_EQUAL(0, sum);

    // todo: add tests for minimum, maximum,
    // todo !!!

   c.destroy();
}
TEST(ColumnFloat_Aggregates) { BasicColumn_Aggregates<ColumnFloat, float>(float_val, float_val_len); }
TEST(ColumnDouble_Aggregates){ BasicColumn_Aggregates<ColumnDouble, double>(double_val, double_val_len); }


template <class C, typename T>
void BasicColumn_Delete(T val[], size_t valLen)
{
    C c;
    for (size_t i=0; i<valLen; ++i)
        c.add(val[i]);
    CHECK_EQUAL(5, c.size());
    CHECK_EQUAL(val[0], c.get(0));
    CHECK_EQUAL(val[1], c.get(1));
    CHECK_EQUAL(val[2], c.get(2));
    CHECK_EQUAL(val[3], c.get(3));
    CHECK_EQUAL(val[4], c.get(4));

    // Delete first
    c.erase(0, 0 == c.size()-1);
    CHECK_EQUAL(4, c.size());
    CHECK_EQUAL(val[1], c.get(0));
    CHECK_EQUAL(val[2], c.get(1));
    CHECK_EQUAL(val[3], c.get(2));
    CHECK_EQUAL(val[4], c.get(3));

    // Delete middle
    c.erase(2, 2 == c.size()-1);
    CHECK_EQUAL(3, c.size());
    CHECK_EQUAL(val[1], c.get(0));
    CHECK_EQUAL(val[2], c.get(1));
    CHECK_EQUAL(val[4], c.get(2));

    // Delete last
    c.erase(2, 2 == c.size()-1);
    CHECK_EQUAL(2, c.size());
    CHECK_EQUAL(val[1], c.get(0));
    CHECK_EQUAL(val[2], c.get(1));

    // Delete single
    c.erase(0, 0 == c.size()-1);
    CHECK_EQUAL(1, c.size());
    CHECK_EQUAL(val[2], c.get(0));

    // Delete all
    c.erase(0, 0 == c.size()-1);
    CHECK_EQUAL(0, c.size());

    c.destroy();
}
TEST(ColumnFloat_Delete) { BasicColumn_Delete<ColumnFloat, float>(float_val, float_val_len); }
TEST(ColumnDouble_Delete){ BasicColumn_Delete<ColumnDouble, double>(double_val, double_val_len); }

TEST(ColumnDouble_InitOfEmptyColumn)
{
    Table t;
    t.add_column(type_Double, "works");
    t.add_column(type_Double, "works also");
    t.add_empty_row();
    t.set_double(0,0,1.1);
    t.set_double(1,0,2.2);
    t.remove_column(1);
    t.add_empty_row();
    t.add_column(type_Double, "doesn't work");
    CHECK_EQUAL(0.0, t.get_double(1,0));
}

TEST(ColumnFloat_InitOfEmptyColumn)
{
    Table t;
    t.add_column(type_Float, "works");
    t.add_column(type_Float, "works also");
    t.add_empty_row();
    t.set_float(0,0,1.1f);
    t.set_float(1,0,2.2f);
    t.remove_column(1);
    t.add_empty_row();
    t.add_column(type_Float, "doesn't work");
    CHECK_EQUAL(0.0, t.get_float(1,0));
}

TEST(ColumnInt_InitOfEmptyColumn)
{
    Table t;
    t.add_column(type_Int, "works");
    t.add_column(type_Int, "works also");
    t.add_empty_row();
    t.set_int(0,0,1);
    t.set_int(1,0,2);
    t.remove_column(1);
    t.add_empty_row();
    t.add_column(type_Int, "doesn't work");
    CHECK_EQUAL(0, t.get_int(1,0));
}

TEST(ColumnString_InitOfEmptyColumn)
{
    Table t;
    t.add_column(type_String, "works");
    t.add_column(type_String, "works also");
    t.add_empty_row();
    t.set_string(0,0, "yellow");
    t.set_string(1,0, "very bright");
    t.remove_column(1);
    t.add_empty_row();
    t.add_column(type_String, "doesn't work");
    CHECK_EQUAL("", t.get_string(1,0));
}

TEST(ColumnBinary_InitOfEmptyColumn)
{
    Table t;
    t.add_column(type_Binary, "works");
    t.add_column(type_Binary, "works also");
    t.add_empty_row();
    t.set_binary(0,0, BinaryData("yellow",10));
    t.set_binary(1,0, BinaryData("very bright", 15));
    t.remove_column(1);
    t.add_empty_row();
    t.add_column(type_Binary, "doesn't work");
    CHECK_EQUAL(BinaryData("",0), t.get_binary(1,0));
}

TEST(ColumnBool_InitOfEmptyColumn)
{
    Table t;
    t.add_column(type_Bool, "works");
    t.add_column(type_Bool, "works also");
    t.add_empty_row();
    t.set_bool(0,0, true);
    t.set_bool(1,0, true);
    t.remove_column(1);
    t.add_empty_row();
    t.add_column(type_Bool, "doesn't work");
    CHECK_EQUAL(false, t.get_bool(1,0));
}

TEST(ColumnMixed_InitOfEmptyColumn)
{
    Table t;
    t.add_column(type_Mixed, "works");
    t.add_column(type_Mixed, "works also");
    t.add_empty_row();
    t.set_mixed(0,0, Mixed(1.1));
    t.set_mixed(1,0, Mixed(2.2));
    t.remove_column(1);
    t.add_empty_row();
    t.add_column(type_Mixed, "doesn't work");
    CHECK_EQUAL(0, t.get_mixed(1,0));
}


#endif // TEST_COLUMN_FLOAT