#pragma once
#include <complexities/complexity_analyzer.h>
#include <libds/adt/table.h>
#include <random>
#include <string>

/**
 * @brief Analyzer for measuring access (find) time complexity in HashTable.
 */
template<typename TableType>
class HashTableAccessAnalyzer : public  ds::utils::ComplexityAnalyzer<TableType>
{
public:
    explicit HashTableAccessAnalyzer(const std::string& name)
        : ds::utils::ComplexityAnalyzer<TableType>(name), rng_(144) {}

protected:
    void growToSize(TableType& table, size_t size) override {
        table.clear();
        for (size_t i = 0; i < size; ++i) {
            table.insert(i, getRandomValue());
        }
    }

    void executeOperation(TableType& table) override {
        if (table.size() == 0) return;
        size_t key = rng_() % table.size();
        volatile auto& value = table.find(key);
        (void)value;
    }

    int getRandomValue() const { return rng_() % 100; }


private:
    mutable std::default_random_engine rng_;
};

template<typename TableType>
class HashTableInsertAnalyzer : public ds::utils::ComplexityAnalyzer<TableType>
{
public:
    explicit HashTableInsertAnalyzer(const std::string& name)
        : ds::utils::ComplexityAnalyzer<TableType>(name), rng_(144) {}

protected:
    void growToSize(TableType& table, size_t size) override {
        table.clear();
        for (size_t i = 0; i < size; ++i) {
            table.insert(i, getRandomValue());
        }
    }

    void executeOperation(TableType& table) override {
        // Insert a new unique key each time
        size_t key = table.size();
        table.insert(key, getRandomValue());
    }

    int getRandomValue() const { return rng_() % 100; }

private:
    mutable std::default_random_engine rng_;
};

class HashTableAnalyzerContainer : public ds::utils::CompositeAnalyzer {
public:
    HashTableAnalyzerContainer()
        : CompositeAnalyzer("hash-table-analyzer") {
        this->addAnalyzer(std::make_unique<
            HashTableAccessAnalyzer<ds::adt::HashTable<int, int>>>("hash-table-access"));
        this->addAnalyzer(std::make_unique<
            HashTableInsertAnalyzer<ds::adt::HashTable<int, int>>>("hash-table-insert"));
    }
};