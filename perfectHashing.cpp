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

    RNG *rng;

public:
    ui len;
    vector<Table> table;
    ui table_size;

    HT(ui size, RNG *ptr_rng)
        : rng(ptr_rng), table_size(size), table(size)
    {
        this->len = 0;

        /* initializes the hash function a and b params  */
        this->a = 1u + ptr_rng->next() % (P - 1);
        this->b = ptr_rng->next() % P;
    }

    pair<int32_t, int32_t> insert(ui k)
    {

        ui pos = ((this->a * k) + this->b) % P % this->table_size;
        int32_t pos_inside = this->query(pos, k);

        if (pos_inside == -1)
        {
            /* checks if the table needs rehashing */
            if (2 * this->len > this->table_size)
            {
                this->rehash();

                /* in case of rehashing, the size and function of the table
                changed so the item's new position must be calculated */
                pos = ((this->a * k) + this->b) % P % this->table_size;
            }

            ++this->len;
            ++this->table[pos].len;

            this->table[pos].list.push_back(k);

            return {pos, (table[pos].len - 1)};
        };

        return {pos, pos_inside};
    }

    int32_t query(ui pos, ui k)
    {
        for (int i = 0; i < this->table[pos].len; i++)
        {
            if (this->table[pos].list[i] == k)
                return i;
        }
        return -1;
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
                    ++new_table[new_pos].len;
                    new_table[new_pos].list.push_back(item);
                }
            }
        }

        this->a = new_a;
        this->b = new_b;
        this->table_size = new_size;

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
    ui table_size;

    /* function params */
    ui a, b;

    HT *normal_ht;

    vector<Perfect_table> table;

    RNG *rng;

public:
    Perfect_HT(HT *ptr_normal_table, RNG *ptr_rng)
        : normal_ht(ptr_normal_table), rng(ptr_rng)
    {
        this->table_size = this->normal_ht->len;

        this->populate_first_level();
        this->populate_second_level();
    }

    void populate_first_level()
    {
        ui sum;

        do
        {
            /* initializes the Ht function a and b params  */
            this->a = 1u + this->rng->next() % (P - 1);
            this->b = this->rng->next() % P;

            /* in case of need rehashing, deletes the old table and initializes a new one */
            this->table = vector<Perfect_table>(table_size);

            for (ui i = 0; i < this->table_size; ++i)
            {
                for (ui k = 0; k < this->normal_ht->table[i].len; k++)
                {

                    ui item = this->normal_ht->table[i].list[k];
                    ui pos = ((this->a * item) + this->b) % P % this->table_size;

                    this->table[pos].list.push_back(item);
                    this->table[pos].len++;
                }
            }

            /* checks if the universal hash function distribuited well the elements.
            if it didnt, tries again with a new hash function */
            sum = 0;
            for (int i = 0; i < this->table_size; i++)
                sum += pow(this->table[i].len, 2);

        }while(sum > 4 * this->table_size);
    }

    pair<int32_t, int32_t> query(ui k)
    {
        /* gets its position in the table */
        ui pos = ((this->a * k) + this->b) % P % this->table_size;
        ui pos_inside = -1;

        if(this->table[pos].len != 0){
            ui size_list = pow(this->table[pos].len, 2) + 1;
            pos_inside = ((this->table[pos].a * k) + this->table[pos].b) % P % size_list;
        }

        if (pos_inside == -1)
            return {-1, -1};

        else {
            if (this->table[pos].list[pos_inside] == k)
                return {pos, pos_inside};

            else
                return {-1, -1};
        }
    };

    void populate_second_level()
    {
        for (int i = 0; i < this->table_size; i++)
        {
            if (this->table[i].len != 0)
            {

                ui list_len = pow(this->table[i].len, 2) + 1;

                int collision = 1;

                while (collision)
                {
                    vector<ui> new_list = vector<ui>(list_len);

                    /* initializes the Ht function that will be applied
                    on this list of keys mapped to the same index  */
                    this->table[i].a = 1u + this->rng->next() % (P - 1);
                    this->table[i].b = this->rng->next() % P;

                    for (int k = 0; k < this->table[i].len; k++)
                    {
                        ui item = this->table[i].list[k];

                        ui pos = ((this->table[i].a * item) + this->table[i].b) % P % list_len;

                        if (new_list[pos] != 0)
                        {
                            collision = true;
                            break;
                        }
                        else
                        {
                            collision = false;
                            new_list[pos] = item;
                        }
                    }
                    /* checks if the perfect params finally were found */
                    if (collision == false)
                        this->table[i].list = new_list;
                }
            }
        }
    }
};

int main()
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
        pair<int32_t, int32_t> output = normal_table->insert(k);

        if ((i % freq_ins) == 0)
            cout << "I " << k << " " << output.first << " " << output.second << endl;
    }

    Perfect_HT *perfect_table = new Perfect_HT(normal_table, rng);
    for (int i = 0; i < nr_queries; i++)
    {
        /* generates a value to query in the normal_table */
        ui k = rng->next() % keys_universe;

        if ((i % freq_qry) == 0)
        {
            pair<int32_t, int32_t> output = perfect_table->query(k);
            cout << "Q " << k << " " << output.first << " " << output.second << endl;
        }
    }

    return 0;
}