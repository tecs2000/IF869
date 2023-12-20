#include <bits/stdc++.h>

using namespace std;

#define ui unsigned int

ui P = (1u << 31u) - 1; /* prime number */

class RNG
{
private:
    ui A;
    ui C;
    ui Last;
    ui seed;

public:
    RNG(ui seed)
    {
        A = 1664525;
        C = 1013904223;
        this->Last = 0;
        this->seed = seed;
    }

    ui next()
    {
        if (this->Last == 0)
        {
            this->Last = this->seed;
            return this->seed;
        }
        else
        {
            this->Last = (this->A * this->Last) + this->C;
            return this->Last;
        }
    }
};

struct Table
{
    int len = 0;
    vector<ui> list;
};

class HT
{
private:
    /* hash function params */
    ui a, b;

    ui table_nr_eltos;

    RNG *rng;

public:
    vector<Table> table;
    ui table_size;

    HT(ui size, RNG *ptr_rng)
        : rng(ptr_rng), table_size(size), table(size)
    {

        /* initializes the hash function a and b params  */
        this->a = 1u + ptr_rng->next() % (P - 1);
        this->b = ptr_rng->next() % P;
    }

    void insert(ui k)
    {
        /* gets its position in the table */
        ui pos = ((this->a * k) + this->b) % P % this->table_size;

        /* checks if the key is in the table */
        if (!this->query(pos, k))
        {
            /* checks if the table needs rehashing */
            if (2 * this->table_nr_eltos > this->table_size)
            {
                this->rehash();

                /* in case of rehashing, the size and function of the table
                changed so the item's new position must be calculated */
                pos = ((this->a * k) + this->b) % P % this->table_size;
            }

            ++table_nr_eltos;
            ++this->table[pos].len;

            this->table[pos].list.push_back(k);
        };
    }

    int query(ui pos, ui k)
    {
        for (auto elto : this->table[pos].list)
        {
            if (elto == k)
                return 1;
        }
        return 0;
    };

    void rehash()
    {
        /* allocates new_table with doble previous one's size */
        ui new_size = 2 * this->table_size + 1;
        vector<Table> new_table = vector<Table>(new_size);

        /* generates new hashing function */
        ui new_a = 1u + this->rng->next() % (P - 1);
        ui new_b = this->rng->next() % P;

        for (int i = 0; i < this->table_size; ++i)
        {
            if (this->table[i].len != 0)
            {

                for (int k = 0; k < this->table[i].len; ++k)
                {

                    /* catches the item and generates its new position in the new table */
                    ui item = this->table[i].list[k];
                    ui new_pos = ((new_a * item) + new_b) % P % new_size;

                    /* inserts the elto in the new table */
                    new_table[new_pos].list.push_back(item);
                }
            }
        }

        this->a = new_a;
        this->b = new_b;
        this->table_size = new_size;

        delete &this->table;
        this->table = new_table;
    }
};

struct Perfect_table
{
    /* hash function params */
    ui a, b;
    ui len = 0;

    vector<ui> list;
};

class Perfect_HT
{
private:
    ui table_nr_eltos;
    ui table_size;

    ui a, b;

    HT *normal_ht;

    vector<Perfect_table> table;

    RNG *rng;

public:
    Perfect_HT(HT *ptr_normal_table, ui size, RNG *ptr_rng)
        : normal_ht(ptr_normal_table),
          rng(ptr_rng),
          table_size(size)
    {     
        this->populate();
        this->second_PHT();
    }

    void populate()
    {   
        /* in case of needing rehashing, deletes old table and initializes new one */
        delete &this->table;
        this->table = vector<Perfect_table>(table_size);

        /* initializes the Ht function a and b params  */
        this->a = 1u + this->rng->next() % (P - 1);
        this->b = this->rng->next() % P;

        this->table_nr_eltos = 0;

        /* copies all keys from the normal table to the perfect table */
        for (ui i = 0; i < this->normal_ht->table_size; ++i)
        {
            for (ui k = 0; k < this->normal_ht->table[i].len; ++k)
            {

                ui item = this->normal_ht->table[i].list[k];
                ui pos = ((this->a * item) + this->b) % P % this->table_size; /* gets its position in the perfect table */

                ++this->table_nr_eltos;
                ++this->table[pos].len;
                this->table[pos].list.push_back(item);
            }
        }

        /* checks if the universal hash function distribuited well the elements
        if she didnt, populates again with a new one */
        ui sum = 0;
        for (int i = 0; i < this->table_size; i++)
            sum += pow(this->table[i].len, 2);
        
        if (sum > 4 * this->table_nr_eltos)
            populate();
    }

    // TODO - implement perfect hashing inside each table's index
    void second_PHT(){

    }
};

void main()
{

    ui seed, keys_universe, hash_size, nr_insertions, nr_queries, freq_ins, freq_qry;
    cin >> seed >> keys_universe >> hash_size >> nr_insertions >> nr_queries >> freq_ins >> freq_qry;

    RNG *rng = new RNG(seed);
    HT *normal_table = new HT(hash_size, rng);

    /* Mapes 0..N-1 keys to table allowing collisions */
    for (int i = 0; i < nr_insertions; i++)
    {
        /* generates a value to insert in the normal_table */
        ui k = rng->next() % keys_universe;
        normal_table->insert(k);
    }
}