#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

using namespace std;

class Product {
    private:
        string name;
        string barcode;
        string category;
        string expiry_date;
        string brand;
        double price;
        int stock;

    public:

        Product();
        ~Product();

    // GETTERS
        const string& getName() const;
        const string& getBarcode() const;
        const string& getCategory() const;
        const string& getExpiry_date() const;
        const string& getBrand() const;
        double getPrice() const;
        int getStock() const;

        // SETTERS
        void setName(const string& name);     
        void setBarcode(const string& barcode);
        void setCategory(const string& category);
        void setExpiry_date(const string& date);
        void setBrand(const string& brand); 
        void setPrice(double price);
        void setStock(int stock);  
};

#endif