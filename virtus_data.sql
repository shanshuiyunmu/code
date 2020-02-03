/*
 Navicat Premium Data Transfer

 Source Server         : mysql
 Source Server Type    : MySQL
 Source Server Version : 50726
 Source Host           : localhost:3306
 Source Schema         : virtus_data

 Target Server Type    : MySQL
 Target Server Version : 50726
 File Encoding         : 65001

 Date: 05/07/2019 19:28:04
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for virtus_table
-- ----------------------------
DROP TABLE IF EXISTS `virtus_table`;
CREATE TABLE `virtus_table`  (
  `virtusname` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `virtusmd5` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of virtus_table
-- ----------------------------
INSERT INTO `virtus_table` VALUES ('peGui.exe', '46ca9f06e33972a71772cb5c6d5014e9');
INSERT INTO `virtus_table` VALUES ('FileCleaner.exe', '8537b15ecc5123eb5013745f10b9d88a');
INSERT INTO `virtus_table` VALUES ('Ollydbg.exe', '705975bf4789dbcdf2f1c7f7f5df7f8f');

SET FOREIGN_KEY_CHECKS = 1;
