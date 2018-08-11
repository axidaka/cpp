########################################
# MySQL Crash Course
# http://www.forta.com/books/0672327120/
# Example table creation scripts
########################################


########################
# Create mysqlbook database
########################
DROP DATABASE mysqlbook;
CREATE DATABASE IF NOT EXISTS mysqlbook;
SHOW DATABASES;
USE mysqlbook;

########################
# Create customers table
########################
CREATE TABLE IF NOT EXISTS customers
(
  cust_id      int       NOT NULL AUTO_INCREMENT,
  cust_name    char(50)  NOT NULL ,
  cust_address char(50)  NULL ,
  cust_city    char(50)  NULL ,
  cust_state   char(5)   NULL ,
  cust_zip     char(10)  NULL ,
  cust_country char(50)  NULL ,
  cust_contact char(50)  NULL ,
  cust_email   char(255) NULL ,
  PRIMARY KEY (cust_id)
) ENGINE=InnoDB;

#########################
# Create orderitems table
#########################
CREATE TABLE IF NOT EXISTS orderitems
(
  order_num  int          NOT NULL ,  # 关联到orders表的order_num
  order_item int          NOT NULL ,  # 订单物品号（在某个订单中的顺序)
  prod_id    char(10)     NOT NULL ,  # 关联到products表的prod_id
  quantity   int          NOT NULL ,
  item_price decimal(8,2) NOT NULL ,
  PRIMARY KEY (order_num, order_item)
) ENGINE=InnoDB;


#####################
# Create orders table
#####################
CREATE TABLE IF NOT EXISTS orders
(
  order_num  int      NOT NULL AUTO_INCREMENT,
  order_date datetime NOT NULL ,
  cust_id    int      NOT NULL ,  # 关联到customers表的cust_id
  PRIMARY KEY (order_num)
) ENGINE=InnoDB;

#######################
# Create products table
#######################
CREATE TABLE IF NOT EXISTS products
(
  prod_id    char(10)      NOT NULL,
  vend_id    int           NOT NULL ,  #关联到vendors表的vend_id
  prod_name  char(255)     NOT NULL ,
  prod_price decimal(8,2)  NOT NULL ,
  prod_desc  text          NULL ,
  PRIMARY KEY(prod_id)
) ENGINE=InnoDB;

######################
# Create vendors table
######################
CREATE TABLE IF NOT EXISTS vendors
(
  vend_id      int      NOT NULL AUTO_INCREMENT,
  vend_name    char(50) NOT NULL ,
  vend_address char(50) NULL ,
  vend_city    char(50) NULL ,
  vend_state   char(5)  NULL ,
  vend_zip     char(10) NULL ,
  vend_country char(50) NULL ,
  PRIMARY KEY (vend_id)
) ENGINE=InnoDB;

###########################
# Create productnotes table
###########################
CREATE TABLE IF NOT EXISTS productnotes
(
  note_id    int           NOT NULL AUTO_INCREMENT,
  prod_id    char(10)      NOT NULL,
  note_date datetime       NOT NULL,
  note_text  text          NULL ,
  PRIMARY KEY(note_id),
  FULLTEXT(note_text)
) ENGINE=MyISAM;


#####################
# Define foreign keys
#####################
ALTER TABLE orderitems ADD CONSTRAINT fk_orderitems_orders FOREIGN KEY (order_num) REFERENCES orders (order_num);
ALTER TABLE orderitems ADD CONSTRAINT fk_orderitems_products FOREIGN KEY (prod_id) REFERENCES products (prod_id);
ALTER TABLE orders ADD CONSTRAINT fk_orders_customers FOREIGN KEY (cust_id) REFERENCES customers (cust_id);
ALTER TABLE products ADD CONSTRAINT fk_products_vendors FOREIGN KEY (vend_id) REFERENCES vendors (vend_id);

SHOW TABLES;
SHOW CREATE TABLE vendors;
SHOW COLUMNS FROM vendors;

SHOW CREATE TABLE products
SHOW COLUMNS FROM products;

SHOW CREATE TABLE customers;
SHOW COLUMNS FROM customers;

SHOW CREATE TABLE orders;
SHOW COLUMNS FROM orders;

SHOW CREATE TABLE orderitems;
SHOW COLUMNS FROM orderitems;

SHOW CREATE TABLE productnotes;
SHOW COLUMNS FROM productnotes;