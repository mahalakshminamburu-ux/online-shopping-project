#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
    int id;
    char name[50];
    float price;
    int quantity;
};

struct Order {
    int id;
    char customerName[50];
    int productId;
    int quantity;
    float totalPrice;
    char status[20];
};

void addProduct();
void displayProducts();
void addToCart();
void displayCart();
void removeFromCart();
void updateQuantity();
void checkout();
void displayOrderSummary();

int main() {
    int choice;
    while (1) {
        printf("\n========== ONLINE SHOPPING CART SYSTEM ==========\n");
        printf("1. Add Product\n");
        printf("2. Display Products\n");
        printf("3. Add to Cart\n");
        printf("4. Display Cart\n");
        printf("5. Remove from Cart\n");
        printf("6. Update Quantity\n");
        printf("7. Checkout\n");
        printf("8. Display Order Summary\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addProduct();
                break;
            case 2:
                displayProducts();
                break;
            case 3:
                addToCart();
                break;
            case 4:
                displayCart();
                break;
            case 5:
                removeFromCart();
                break;
            case 6:
                updateQuantity();
                break;
            case 7:
                checkout();
                break;
            case 8:
                displayOrderSummary();
                break;
            case 9:
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

void addProduct() {
    FILE *fp;
    struct Product p;
    fp = fopen("products.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("Enter Product ID: ");
    scanf("%d", &p.id);
    printf("Enter Product Name: ");
    scanf("%s", p.name);
    printf("Enter Price: ");
    scanf("%f", &p.price);
    printf("Enter Quantity: ");
    scanf("%d", &p.quantity);
    fprintf(fp, "%d %s %.2f %d\n", p.id, p.name, p.price, p.quantity);
    fclose(fp);
    printf("Product added successfully!\n");
}

void displayProducts() {
    FILE *fp;
    struct Product p;
    fp = fopen("products.txt", "r");
    if (fp == NULL) {
        printf("No products found!\n");
        return;
    }
    printf("\n%-5s %-20s %-10s %-10s\n", "ID", "Name", "Price", "Quantity");
    printf("-----------------------------------------------\n");
    while (fscanf(fp, "%d %s %f %d", &p.id, p.name, &p.price, &p.quantity) == 4) {
        printf("%-5d %-20s %-10.2f %-10d\n", p.id, p.name, p.price, p.quantity);
    }
    fclose(fp);
}

void addToCart() {
    FILE *fp, *cart;
    struct Product p;
    struct Order o;
    int id, quantity;
    fp = fopen("products.txt", "r");
    cart = fopen("cart.txt", "a");
    if (fp == NULL || cart == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("Enter Product ID: ");
    scanf("%d", &id);
    printf("Enter Quantity: ");
    scanf("%d", &quantity);
    while (fscanf(fp, "%d %s %f %d", &p.id, p.name, &p.price, &p.quantity) == 4) {
        if (p.id == id) {
            if (p.quantity >= quantity) {
                o.productId = p.id;
                strcpy(o.customerName, "John Doe");
                o.quantity = quantity;
                o.totalPrice = p.price * quantity;
                strcpy(o.status, "Pending");
                fprintf(cart, "%d %s %d %.2f %s\n", o.productId, o.customerName, o.quantity, o.totalPrice, o.status);
                printf("Product added to cart successfully!\n");
            } else {
                printf("Insufficient quantity!\n");
            }
            break;
        }
    }
    fclose(fp);
    fclose(cart);
}

void displayCart() {
    FILE *cart;
    struct Order o;
    cart = fopen("cart.txt", "r");
    if (cart == NULL) {
        printf("No products in cart!\n");
        return;
    }
    printf("\n%-5s %-20s %-10s %-10s %-10s\n", "ID", "Customer Name", "Quantity", "Total Price", "Status");
    printf("---------------------------------------------------------------\n");
    while (fscanf(cart, "%d %s %d %f %s", &o.productId, o.customerName, &o.quantity, &o.totalPrice, o.status) == 5) {
        printf("%-5d %-20s %-10d %-10.2f %-10s\n", o.productId, o.customerName, o.quantity, o.totalPrice, o.status);
    }
    fclose(cart);
}

void removeFromCart() {
    FILE *cart, *temp;
    struct Order o;
    int id;
    cart = fopen("cart.txt", "r");
    temp = fopen("temp.txt", "w");
    if (cart == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("Enter Product ID to remove: ");
    scanf("%d", &id);
    while (fscanf(cart, "%d %s %d %f %s", &o.productId, o.customerName, &o.quantity, &o.totalPrice, o.status) == 5) {
        if (o.productId != id) {
            fprintf(temp, "%d %s %d %.2f %s\n", o.productId, o.customerName, o.quantity, o.totalPrice, o.status);
        }
    }
    fclose(cart);
    fclose(temp);
    remove("cart.txt");
    rename("temp.txt", "cart.txt");
    printf("Product removed from cart successfully!\n");
}

void updateQuantity() {
    FILE *cart, *temp;
    struct Order o;
    int id, quantity;
    cart = fopen("cart.txt", "r");
    temp = fopen("temp.txt", "w");
    if (cart == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    printf("Enter Product ID to update: ");
    scanf("%d", &id);
    printf("Enter new quantity: ");
    scanf("%d", &quantity);
    while (fscanf(cart, "%d %s %d %f %s", &o.productId, o.customerName, &o.quantity, &o.totalPrice, o.status) == 5) {
        if (o.productId == id) {
            o.quantity = quantity;
            o.totalPrice = o.totalPrice / o.quantity * quantity;
            fprintf(temp, "%d %s %d %.2f %s\n", o.productId, o.customerName, o.quantity, o.totalPrice, o.status);
        } else {
            fprintf(temp, "%d %s %d %.2f %s\n", o.productId, o.customerName, o.quantity, o.totalPrice, o.status);
        }
    }
    fclose(cart);
    fclose(temp);
    remove("cart.txt");
    rename("temp.txt", "cart.txt");
    printf("Quantity updated successfully!\n");
}

void checkout() {
    FILE *cart, *orders;
    struct Order o;
    cart = fopen("cart.txt", "r");
    orders = fopen("orders.txt", "a");
    if (cart == NULL || orders == NULL) {
        printf("Error opening file!\n");
        return;
    }
    while (fscanf(cart, "%d %s %d %f %s", &o.productId, o.customerName, &o.quantity, &o.totalPrice, o.status) == 5) {
        fprintf(orders, "%d %s %d %.2f %s\n", o.productId, o.customerName, o.quantity, o.totalPrice, o.status);
    }
    fclose(cart);
    fclose(orders);
    remove("cart.txt");
    printf("Order placed successfully!\n");
}

void displayOrderSummary() {
    FILE *orders;
    struct Order o;
    orders = fopen("orders.txt", "r");
    if (orders == NULL) {
        printf("No orders found!\n");
        return;
    }
    printf("\n%-5s %-20s %-10s %-10s %-10s\n", "ID", "Customer Name", "Quantity", "Total Price", "Status");
    printf("---------------------------------------------------------------\n");
    while (fscanf(orders, "%d %s %d %f %s", &o.productId, o.customerName, &o.quantity, &o.totalPrice, o.status) == 5) {
        printf("%-5d %-20s %-10d %-10.2f %-10s\n", o.productId, o.customerName, o.quantity, o.totalPrice, o.status);
    }
    fclose(orders);
}
