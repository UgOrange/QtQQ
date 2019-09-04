-- MySQL dump 10.13  Distrib 5.7.27, for Linux (x86_64)
--
-- Host: localhost    Database: QQ_Database
-- ------------------------------------------------------
-- Server version	5.7.27-0ubuntu0.16.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `QQ_Database`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `QQ_Database` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `QQ_Database`;

--
-- Table structure for table `friend_apply`
--

DROP TABLE IF EXISTS `friend_apply`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `friend_apply` (
  `object_id` int(5) NOT NULL,
  `applicant_id` int(5) NOT NULL,
  `friend_apply_message` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`object_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `friend_apply`
--

LOCK TABLES `friend_apply` WRITE;
/*!40000 ALTER TABLE `friend_apply` DISABLE KEYS */;
INSERT INTO `friend_apply` VALUES (12,5,NULL),(13,5,NULL);
/*!40000 ALTER TABLE `friend_apply` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `friend_info`
--

DROP TABLE IF EXISTS `friend_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `friend_info` (
  `friend_id` int(5) NOT NULL,
  `userid` int(5) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `friend_info`
--

LOCK TABLES `friend_info` WRITE;
/*!40000 ALTER TABLE `friend_info` DISABLE KEYS */;
INSERT INTO `friend_info` VALUES (123,133),(123,233);
/*!40000 ALTER TABLE `friend_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `group_chat_history`
--

DROP TABLE IF EXISTS `group_chat_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_chat_history` (
  `group_chat_info_id` int(11) NOT NULL,
  `time` varchar(64) NOT NULL,
  `read` varchar(45) NOT NULL DEFAULT '0',
  `poster_id` int(5) NOT NULL,
  `content` varchar(200) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `group_chat_history`
--

LOCK TABLES `group_chat_history` WRITE;
/*!40000 ALTER TABLE `group_chat_history` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_chat_history` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `group_chat_info`
--

DROP TABLE IF EXISTS `group_chat_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_chat_info` (
  `group_chat_info_id` int(11) NOT NULL,
  `member_id` int(5) NOT NULL,
  `group_chat_name` varchar(45) DEFAULT NULL,
  `group_chat_admin` tinyint(1) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `group_chat_info`
--

LOCK TABLES `group_chat_info` WRITE;
/*!40000 ALTER TABLE `group_chat_info` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_chat_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `group_chat_management`
--

DROP TABLE IF EXISTS `group_chat_management`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_chat_management` (
  `applicant_id` int(5) NOT NULL,
  `group_chat_id` varchar(45) NOT NULL,
  `group_chat_management_message` varchar(200) DEFAULT '""',
  PRIMARY KEY (`group_chat_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `group_chat_management`
--

LOCK TABLES `group_chat_management` WRITE;
/*!40000 ALTER TABLE `group_chat_management` DISABLE KEYS */;
/*!40000 ALTER TABLE `group_chat_management` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `photo`
--

DROP TABLE IF EXISTS `photo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `photo` (
  `poster_id` int(5) NOT NULL,
  `photo_content` blob NOT NULL,
  `time` time(6) NOT NULL,
  PRIMARY KEY (`poster_id`,`time`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `photo`
--

LOCK TABLES `photo` WRITE;
/*!40000 ALTER TABLE `photo` DISABLE KEYS */;
/*!40000 ALTER TABLE `photo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `single_chat_history`
--

DROP TABLE IF EXISTS `single_chat_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `single_chat_history` (
  `chat_info_id` int(11) NOT NULL,
  `time` varchar(64) NOT NULL,
  `photo` tinyint(1) NOT NULL,
  `read` tinyint(1) NOT NULL DEFAULT '0',
  `poster_id` int(5) unsigned zerofill NOT NULL,
  `content` varchar(200) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `single_chat_history`
--

LOCK TABLES `single_chat_history` WRITE;
/*!40000 ALTER TABLE `single_chat_history` DISABLE KEYS */;
/*!40000 ALTER TABLE `single_chat_history` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `single_chat_info`
--

DROP TABLE IF EXISTS `single_chat_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `single_chat_info` (
  `single_chat_info_id` int(11) unsigned zerofill NOT NULL AUTO_INCREMENT,
  `member_id1` int(5) NOT NULL,
  `member_id2` int(5) NOT NULL,
  PRIMARY KEY (`single_chat_info_id`)
) ENGINE=InnoDB AUTO_INCREMENT=113 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `single_chat_info`
--

LOCK TABLES `single_chat_info` WRITE;
/*!40000 ALTER TABLE `single_chat_info` DISABLE KEYS */;
INSERT INTO `single_chat_info` VALUES (00000000111,123,321),(00000000112,321,123);
/*!40000 ALTER TABLE `single_chat_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `temperary_message`
--

DROP TABLE IF EXISTS `temperary_message`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `temperary_message` (
  `poster_id` int(5) NOT NULL,
  `recv_id` int(5) NOT NULL,
  `time` varchar(64) NOT NULL,
  `content` varchar(200) NOT NULL,
  `group_chat_info_id` int(11) unsigned zerofill NOT NULL DEFAULT '00000000000'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `temperary_message`
--

LOCK TABLES `temperary_message` WRITE;
/*!40000 ALTER TABLE `temperary_message` DISABLE KEYS */;
/*!40000 ALTER TABLE `temperary_message` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `userid` int(5) unsigned zerofill NOT NULL AUTO_INCREMENT,
  `user_name` varchar(45) NOT NULL,
  `password` varchar(45) NOT NULL,
  `nickname` varchar(45) DEFAULT NULL,
  `signature` varchar(60) DEFAULT NULL,
  `online_status` tinyint(1) DEFAULT '0',
  `head_portrait_id` varchar(64) DEFAULT NULL,
  `VIP` tinyint(1) NOT NULL DEFAULT '0',
  `token` varchar(32) DEFAULT NULL,
  `password_protect_id` int(1) unsigned NOT NULL,
  `answer` varchar(32) NOT NULL,
  PRIMARY KEY (`userid`),
  UNIQUE KEY `user_name` (`user_name`)
) ENGINE=InnoDB AUTO_INCREMENT=124 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES (00123,'andy','123','amy','hahaha',0,'5',1,NULL,4,'yes');
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `QQ_Database`.`user_BEFORE_INSERT` BEFORE INSERT ON `user` FOR EACH ROW
BEGIN

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-09-04 11:17:18
