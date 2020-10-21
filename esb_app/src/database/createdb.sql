
    DROP DATABASE IF EXISTS `esb`;

    CREATE DATABSE IF NOT EXISTS `esb`; 

    USE `esb`;

    --Table creation

    DROP TABLE IF EXISTS `esb`.`esb_request`;

    CREATE TABLE IF NOT EXISTS `esb`.`esb_request` (
        `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY,
        `sender_id` VARCHAR(45) NOT NULL,
        `dest_id` VARCHAR(45) NOT NULL,
        `message_type` VARCHAR(45) NOT NULL,
        `reference_id` VARCHAR(45) NOT NULL,
        `message_id` VARCHAR(45) NOT NULL COMMENT 'A unique ID for the message instance.',
        `received_on` DATETIME NOT NULL,
        `data_location` TEXT,
        `status` VARCHAR(20) NOT NULL,
        `status_details` TEXT 
    );
