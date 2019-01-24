-- phpMyAdmin SQL Dump
-- version 4.8.3
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Gegenereerd op: 19 dec 2018 om 13:00
-- Serverversie: 5.6.42
-- PHP-versie: 7.0.32

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `sustainablecharging_nl_evcharging`
--

-- --------------------------------------------------------

--
-- Tabelstructuur voor tabel `measurements`
--

CREATE TABLE `measurements` (
  `id` int(11) NOT NULL,
  `phase_voltage_L1` float NOT NULL,
  `phase_voltage_L2` float NOT NULL,
  `phase_voltage_L3` float NOT NULL,
  `current_L1` float NOT NULL,
  `current_L2` float NOT NULL,
  `current_L3` float NOT NULL,
  `active_power` float NOT NULL,
  `energy` float NOT NULL,
  `frequency` float NOT NULL,
  `createdAt` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `SoC` float NOT NULL,
  `socketId` int(11) NOT NULL,
  `uidTag` varchar(45) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Indexen voor geëxporteerde tabellen
--

--
-- Indexen voor tabel `measurements`
--
ALTER TABLE `measurements`
  ADD PRIMARY KEY (`id`),
  ADD KEY `fk_measurements_2_idx` (`socketId`),
  ADD KEY `fk_measurements_1` (`uidTag`);

--
-- AUTO_INCREMENT voor geëxporteerde tabellen
--

--
-- AUTO_INCREMENT voor een tabel `measurements`
--
ALTER TABLE `measurements`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- Beperkingen voor geëxporteerde tabellen
--

--
-- Beperkingen voor tabel `measurements`
--
ALTER TABLE `measurements`
  ADD CONSTRAINT `fk_measurements_1` FOREIGN KEY (`uidTag`) REFERENCES `users` (`uidTag`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `fk_measurements_2` FOREIGN KEY (`socketId`) REFERENCES `charging_station_sockets` (`id`) ON DELETE NO ACTION ON UPDATE NO ACTION;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
