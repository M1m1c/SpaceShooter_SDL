#pragma once
#include <tuple>
#include <array>
#include "ECSCore.h"

//template<size_t size, typename... Types>
//using DataTable = std::tuple<std::array<Entity, size>, std::array <CompSignature, size>, std::array <Types, size>...>;

//template<size_t size, typename... Types>
//struct DataTable
//{
//    using type = std::tuple<std::array<Entity, size>, std::array<CompSignature, size>, std::array<Types, size>...>;
//};

template<size_t size, typename... Types>
struct DataTable
{
    using type = std::tuple<std::array<Entity, size>, std::array<CompSignature, size>, std::array<std::unique_ptr<Types>, size>...>;

    // Constructor that initializes the unique_ptr elements with nullptr
    DataTable()
    {
        (std::get<2 + sizeof...(Types)>(data) = std::make_unique<Types>(nullptr), ...);
    }

    // Method to set the value of an element in the DataTable
    template<size_t index, typename T>
    void set(T&& value)
    {
        std::get<2 + sizeof...(Types)>(data)[index] = std::make_unique<T>(std::forward<T>(value));
    }

    // Method to get the value of an element in the DataTable
    template<size_t index, typename T>
    T& get()
    {
        return *std::get<2 + sizeof...(Types)>(data)[index];
    }

    type data;
};

//template<size_t size, typename... Types>
//struct DataTable
//{
//    using type = std::tuple<std::array<Entity, size>, std::array<CompSignature, size>, std::array<std::vector<Types>, size>>;
//
//    // Method to set the size of an array in the DataTable
//    template<size_t index>
//    void set_size(size_t new_size)
//    {
//        std::get<2>(data)[index].resize(new_size);
//    }
//
//    // Method to set the value of an element in the DataTable
//    template<size_t index, typename T>
//    void set(size_t element_index, T&& value)
//    {
//        std::get<2>(data)[index][element_index] = std::forward<T>(value);
//    }
//
//    // Method to get the value of an element in the DataTable
//    template<size_t index, typename T>
//    T& get(size_t element_index)
//    {
//        return std::get<2>(data)[index][element_index];
//    }
//
//    type data;
//};


//template<size_t size, typename... Types>
//struct DataTable
//{
//    using type = std::tuple<std::array<Entity, size>, std::array<CompSignature, size>, std::array<std::variant<Types...>, size>>;
//
//    // Method to set the value of an element in the DataTable
//    template<size_t index, typename T>
//    void set(T&& value)
//    {
//        std::get<2>(data)[index] = std::forward<T>(value);
//    }
//
//    // Method to get the value of an element in the DataTable
//    template<size_t index, typename T>
//    T& get()
//    {
//        return std::get<T>(std::get<2>(data)[index]);
//    }
//
//    type data;
//};
