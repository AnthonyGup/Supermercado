#include "Product.h"

Product::Product()
    : price(0.0), stock(0) {}

Product::~Product() = default;

// GETTERS

const string &Product::getName() const
{
    return this->name;
}

const string &Product::getBarcode() const
{
    return this->barcode;
}

const string &Product::getCategory() const
{
    return this->category;
}

const string &Product::getExpiry_date() const
{
    return this->expiry_date;
}

const string &Product::getBrand() const
{
    return this->brand;
}

double Product::getPrice() const
{
    return this->price;
}

int Product::getStock() const
{
    return this->stock;
}

// SETTERS

void Product::setName(const string &name)
{
    this->name = name;
}

void Product::setBarcode(const string &barcode)
{
    this->barcode = barcode;
}

void Product::setCategory(const string &category)
{
    this->category = category;
}

void Product::setExpiry_date(const string &date)
{
    this->expiry_date = date;
}

void Product::setBrand(const string &brand)
{
    this->brand = brand;
}

void Product::setPrice(double price)
{
    this->price = price;
}

void Product::setStock(int stock)
{
    this->stock = stock;
}
