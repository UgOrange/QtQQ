-- MariaDB dump 10.17  Distrib 10.4.7-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: chatroom
-- ------------------------------------------------------
-- Server version	10.4.7-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `chat_info`
--

DROP TABLE IF EXISTS `chat_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chat_info` (
  `chat_info_id` int(11) unsigned zerofill NOT NULL AUTO_INCREMENT,
  `member_id` int(5) NOT NULL,
  PRIMARY KEY (`chat_info_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chat_info`
--

LOCK TABLES `chat_info` WRITE;
/*!40000 ALTER TABLE `chat_info` DISABLE KEYS */;
/*!40000 ALTER TABLE `chat_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `friend_apply`
--

DROP TABLE IF EXISTS `friend_apply`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `friend_apply` (
  `object_id` int(5) NOT NULL,
  `applicant_id` int(5) NOT NULL,
  PRIMARY KEY (`object_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `friend_apply`
--

LOCK TABLES `friend_apply` WRITE;
/*!40000 ALTER TABLE `friend_apply` DISABLE KEYS */;
INSERT INTO `friend_apply` VALUES (12,5),(13,5);
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
  `userid` int(5) NOT NULL,
  PRIMARY KEY (`friend_id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `friend_info`
--

LOCK TABLES `friend_info` WRITE;
/*!40000 ALTER TABLE `friend_info` DISABLE KEYS */;
/*!40000 ALTER TABLE `friend_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `group_chat_history`
--

DROP TABLE IF EXISTS `group_chat_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_chat_history` (
  `group_chat_history_id` int(11) NOT NULL,
  `time` timestamp(6) NOT NULL DEFAULT current_timestamp(6) ON UPDATE current_timestamp(6),
  `poster_id` int(5) NOT NULL,
  `content` varchar(200) NOT NULL,
  PRIMARY KEY (`group_chat_history_id`)
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
  PRIMARY KEY (`group_chat_info_id`)
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
  PRIMARY KEY (`applicant_id`)
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
  `photo_id` int(5) NOT NULL,
  `photo_content` blob NOT NULL,
  PRIMARY KEY (`photo_id`)
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
  `time` timestamp(6) NOT NULL DEFAULT current_timestamp(6) ON UPDATE current_timestamp(6),
  `poster_id` int(5) unsigned zerofill NOT NULL,
  `content` varchar(200) NOT NULL,
  PRIMARY KEY (`chat_info_id`)
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
  `online_status` tinyint(1) DEFAULT 0,
  `head_portrait_id` int(5) DEFAULT NULL,
  `VIP` tinyint(1) NOT NULL,
  `token` varchar(32) DEFAULT NULL,
  `password_protect_id` int(4) unsigned zerofill NOT NULL,
  `answer` varchar(32) NOT NULL,
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES (00001,'andy','12345678','ng','good',0,15,0,NULL,0000,''),(00002,'andy','12345678','ng','good',0,15,0,NULL,0000,''),(00003,'lpx','123546','pp','bad',0,16,0,NULL,0001,'no'),(00004,'lpx','123546','pp','bad',0,16,0,NULL,0001,'no');
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-09-01  9:51:51
