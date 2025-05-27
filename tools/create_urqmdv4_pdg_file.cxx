#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <utility>

using std::make_pair;

int main() {
  //   std::unordered_map<std::pair<int, int>, std::pair<int, std::string>>
  //   urqmd2pdg;
  std::map<std::pair<int, int>, std::pair<int, std::string>> urqmd2pdg;

  // Nucleon and Nucleon resonances
  urqmd2pdg[make_pair(1, -1)] = make_pair(2112, "Neutron");
  urqmd2pdg[make_pair(1, 1)] = make_pair(2212, "Proton");

  urqmd2pdg[make_pair(2, -1)] = make_pair(12112, "N(1440)0");
  urqmd2pdg[make_pair(2, 1)] = make_pair(12212, "N(1440)+");
  urqmd2pdg[make_pair(3, -1)] = make_pair(1214, "N(1520)0");
  urqmd2pdg[make_pair(3, 1)] = make_pair(2124, "N(1520)+");
  urqmd2pdg[make_pair(4, -1)] = make_pair(22112, "N(1535)0");
  urqmd2pdg[make_pair(4, 1)] = make_pair(22212, "N(1535)+");
  urqmd2pdg[make_pair(5, -1)] = make_pair(32112, "N(1650)0");
  urqmd2pdg[make_pair(5, 1)] = make_pair(32212, "N(1650)+");
  urqmd2pdg[make_pair(6, -1)] = make_pair(2116, "N(1675)0");
  urqmd2pdg[make_pair(6, 1)] = make_pair(2216, "N(1675)+");
  urqmd2pdg[make_pair(7, -1)] = make_pair(12116, "N(1680)0");
  urqmd2pdg[make_pair(7, 1)] = make_pair(12216, "N(1680)+");
  urqmd2pdg[make_pair(8, -1)] = make_pair(21214, "N(1700)0");
  urqmd2pdg[make_pair(8, 1)] = make_pair(22124, "N(1700)+");
  urqmd2pdg[make_pair(9, -1)] = make_pair(42112, "N(1710)0");
  urqmd2pdg[make_pair(9, 1)] = make_pair(42212, "N(1710)+");
  urqmd2pdg[make_pair(10, -1)] = make_pair(31214, "N(1720)0");
  urqmd2pdg[make_pair(10, 1)] = make_pair(32124, "N(1720)+");
  urqmd2pdg[make_pair(11, -1)] = make_pair(41214, "N(1900)0");
  urqmd2pdg[make_pair(11, 1)] = make_pair(42124, "N(1900)+");
  urqmd2pdg[make_pair(12, -1)] = make_pair(12118, "N(1990)0");
  urqmd2pdg[make_pair(12, 1)] = make_pair(12218, "N(1990)+");
  urqmd2pdg[make_pair(13, -1)] = make_pair(52114, "N(2090)0");
  urqmd2pdg[make_pair(13, 1)] = make_pair(52214, "N(2090)+");
  urqmd2pdg[make_pair(14, -1)] = make_pair(1218, "N(2190)0");
  urqmd2pdg[make_pair(14, 1)] = make_pair(2128, "N(2190)+");
  urqmd2pdg[make_pair(15, -1)] = make_pair(100002110, "N(2220)0");
  urqmd2pdg[make_pair(15, 1)] = make_pair(100002210, "N(2220)+");
  urqmd2pdg[make_pair(16, -1)] = make_pair(100012110, "N(2250)0");
  urqmd2pdg[make_pair(16, 1)] = make_pair(100012210, "N(2250)+");

  urqmd2pdg[make_pair(17, -1)] = make_pair(0, "N(2600)0");
  urqmd2pdg[make_pair(17, 1)] = make_pair(0, "N(2600)+");
  urqmd2pdg[make_pair(18, -1)] = make_pair(0, "N(1875)0");
  urqmd2pdg[make_pair(18, 1)] = make_pair(0, "N(1875)+");
  urqmd2pdg[make_pair(19, -1)] = make_pair(0, "N(2700)0");
  urqmd2pdg[make_pair(19, 1)] = make_pair(0, "N(2700)+");
  urqmd2pdg[make_pair(20, -1)] = make_pair(0, "N(3100)0");
  urqmd2pdg[make_pair(20, 1)] = make_pair(0, "N(3100)+");
  urqmd2pdg[make_pair(21, -1)] = make_pair(0, "N(3500)0");
  urqmd2pdg[make_pair(21, 1)] = make_pair(0, "N(3500)+");
  urqmd2pdg[make_pair(22, -1)] = make_pair(0, "N(3800)0");
  urqmd2pdg[make_pair(22, 1)] = make_pair(0, "N(3800)+");
  urqmd2pdg[make_pair(23, -1)] = make_pair(0, "N(4100)0");
  urqmd2pdg[make_pair(23, 1)] = make_pair(0, "N(4100)+");

  // Deltas
  urqmd2pdg[make_pair(24, -3)] = make_pair(1114, "delta-");
  urqmd2pdg[make_pair(24, -1)] = make_pair(2114, "delta0");
  urqmd2pdg[make_pair(24, 1)] = make_pair(2214, "delta+");
  urqmd2pdg[make_pair(24, 3)] = make_pair(2224, "delta++");
  urqmd2pdg[make_pair(25, -3)] = make_pair(31114, "delta(1600)-");
  urqmd2pdg[make_pair(25, -1)] = make_pair(32114, "delta(1600)0");
  urqmd2pdg[make_pair(25, 1)] = make_pair(32214, "delta(1600)+");
  urqmd2pdg[make_pair(25, 3)] = make_pair(32224, "delta(1600)++");
  urqmd2pdg[make_pair(26, -3)] = make_pair(1112, "delta(1620)-");
  urqmd2pdg[make_pair(26, -1)] = make_pair(1212, "delta(1620)0");
  urqmd2pdg[make_pair(26, 1)] = make_pair(2122, "delta(1620)+");
  urqmd2pdg[make_pair(26, 3)] = make_pair(2222, "delta(1620)++");
  urqmd2pdg[make_pair(27, -3)] = make_pair(11114, "delta(1700)-");
  urqmd2pdg[make_pair(27, -1)] = make_pair(12114, "delta(1700)0");
  urqmd2pdg[make_pair(27, 1)] = make_pair(12214, "delta(1700)+");
  urqmd2pdg[make_pair(27, 3)] = make_pair(12224, "delta(1700)++");
  urqmd2pdg[make_pair(28, -3)] = make_pair(11112, "delta(1900)-");
  urqmd2pdg[make_pair(28, -1)] = make_pair(11212, "delta(1900)0");
  urqmd2pdg[make_pair(28, 1)] = make_pair(12122, "delta(1900)+");
  urqmd2pdg[make_pair(28, 3)] = make_pair(12222, "delta(1900)++");
  urqmd2pdg[make_pair(29, -3)] = make_pair(1116, "delta(1905)-");
  urqmd2pdg[make_pair(29, -1)] = make_pair(1216, "delta(1905)0");
  urqmd2pdg[make_pair(29, 1)] = make_pair(2126, "delta(1905)+");
  urqmd2pdg[make_pair(29, 3)] = make_pair(2226, "delta(1905)++");
  urqmd2pdg[make_pair(30, -3)] = make_pair(21112, "delta(1910)-");
  urqmd2pdg[make_pair(30, -1)] = make_pair(21212, "delta(1910)0");
  urqmd2pdg[make_pair(30, 1)] = make_pair(22122, "delta(1910)+");
  urqmd2pdg[make_pair(30, 3)] = make_pair(22222, "delta(1910)++");
  urqmd2pdg[make_pair(31, -3)] = make_pair(21114, "delta(1920)-");
  urqmd2pdg[make_pair(31, -1)] = make_pair(22114, "delta(1920)0");
  urqmd2pdg[make_pair(31, 1)] = make_pair(22214, "delta(1920)+");
  urqmd2pdg[make_pair(31, 3)] = make_pair(22224, "delta(1920)++");
  urqmd2pdg[make_pair(32, -3)] = make_pair(11116, "delta(1930)-");
  urqmd2pdg[make_pair(32, -1)] = make_pair(11216, "delta(1930)0");
  urqmd2pdg[make_pair(32, 1)] = make_pair(12126, "delta(1930)+");
  urqmd2pdg[make_pair(32, 3)] = make_pair(12226, "delta(1930)++");
  urqmd2pdg[make_pair(33, -3)] = make_pair(1118, "delta(1950)-");
  urqmd2pdg[make_pair(33, -1)] = make_pair(2118, "delta(1950)0");
  urqmd2pdg[make_pair(33, 1)] = make_pair(2218, "delta(1950)+");
  urqmd2pdg[make_pair(33, 3)] = make_pair(2228, "delta(1950)++");
  urqmd2pdg[make_pair(34, -3)] = make_pair(0, "delta(2420)-");
  urqmd2pdg[make_pair(34, -1)] = make_pair(0, "delta(2420)0");
  urqmd2pdg[make_pair(34, 1)] = make_pair(0, "delta(2420)+");
  urqmd2pdg[make_pair(34, 3)] = make_pair(0, "delta(2420)++");
  urqmd2pdg[make_pair(35, -3)] = make_pair(0, "delta(2750)-");
  urqmd2pdg[make_pair(35, -1)] = make_pair(0, "delta(2750)0");
  urqmd2pdg[make_pair(35, 1)] = make_pair(0, "delta(2750)+");
  urqmd2pdg[make_pair(35, 3)] = make_pair(0, "delta(2750)++");
  urqmd2pdg[make_pair(36, -3)] = make_pair(0, "delta(2950)-");
  urqmd2pdg[make_pair(36, -1)] = make_pair(0, "delta(2950)0");
  urqmd2pdg[make_pair(36, 1)] = make_pair(0, "delta(2950)+");
  urqmd2pdg[make_pair(36, 3)] = make_pair(0, "delta(2950)++");
  urqmd2pdg[make_pair(37, -3)] = make_pair(0, "delta(3300)-");
  urqmd2pdg[make_pair(37, -1)] = make_pair(0, "delta(3300)0");
  urqmd2pdg[make_pair(37, 1)] = make_pair(0, "delta(3300)+");
  urqmd2pdg[make_pair(37, 3)] = make_pair(0, "delta(3300)++");
  urqmd2pdg[make_pair(38, -3)] = make_pair(0, "delta(3500)-");
  urqmd2pdg[make_pair(38, -1)] = make_pair(0, "delta(3500)0");
  urqmd2pdg[make_pair(38, 1)] = make_pair(0, "delta(3500)+");
  urqmd2pdg[make_pair(38, 3)] = make_pair(0, "delta(3500)++");
  urqmd2pdg[make_pair(39, -3)] = make_pair(0, "delta(3700)-");
  urqmd2pdg[make_pair(39, -1)] = make_pair(0, "delta(3700)0");
  urqmd2pdg[make_pair(39, 1)] = make_pair(0, "delta(3700)+");
  urqmd2pdg[make_pair(39, 3)] = make_pair(0, "delta(3700)++");
  urqmd2pdg[make_pair(40, -3)] = make_pair(0, "delta(3700)-");
  urqmd2pdg[make_pair(40, -1)] = make_pair(0, "delta(3700)0");
  urqmd2pdg[make_pair(40, 1)] = make_pair(0, "delta(3700)+");
  urqmd2pdg[make_pair(40, 3)] = make_pair(0, "delta(3700)++");

  // Lambdas
  urqmd2pdg[make_pair(41, 0)] = make_pair(3122, "lambda");
  urqmd2pdg[make_pair(42, 0)] = make_pair(13122, "lambda(1405)");
  urqmd2pdg[make_pair(43, 0)] = make_pair(3124, "lambda(1520)");
  urqmd2pdg[make_pair(44, 0)] = make_pair(23122, "lambda(1600)");
  urqmd2pdg[make_pair(45, 0)] = make_pair(33122, "lambda(1670)");
  urqmd2pdg[make_pair(46, 0)] = make_pair(13124, "lambda(1690)");
  urqmd2pdg[make_pair(47, 0)] = make_pair(43122, "lambda(1800)");
  urqmd2pdg[make_pair(48, 0)] = make_pair(53122, "lambda(1810)");
  urqmd2pdg[make_pair(49, 0)] = make_pair(3126, "lambda(1820)");
  urqmd2pdg[make_pair(50, 0)] = make_pair(13126, "lambda(1830)");
  urqmd2pdg[make_pair(51, 0)] = make_pair(23124, "lambda(1890)");
  urqmd2pdg[make_pair(52, 0)] = make_pair(3128, "lambda(2100)");
  urqmd2pdg[make_pair(53, 0)] = make_pair(23126, "lambda(2110)");

  // Sigmas
  urqmd2pdg[make_pair(54, -2)] = make_pair(3112, "sigma-");
  urqmd2pdg[make_pair(54, 0)] = make_pair(3212, "sigma0");
  urqmd2pdg[make_pair(54, 2)] = make_pair(3222, "sigma+");
  urqmd2pdg[make_pair(55, -2)] = make_pair(3114, "sigma(1385)-");
  urqmd2pdg[make_pair(55, 0)] = make_pair(3214, "sigma(1385)0");
  urqmd2pdg[make_pair(55, 2)] = make_pair(3224, "sigma(1385)+");
  urqmd2pdg[make_pair(56, -2)] = make_pair(13112, "sigma(1660)-");
  urqmd2pdg[make_pair(56, 0)] = make_pair(13212, "sigma(1660)0");
  urqmd2pdg[make_pair(56, 2)] = make_pair(13222, "sigma(1660)+");
  urqmd2pdg[make_pair(57, -2)] = make_pair(13114, "sigma(1670)-");
  urqmd2pdg[make_pair(57, 0)] = make_pair(13214, "sigma(1670)0");
  urqmd2pdg[make_pair(57, 2)] = make_pair(13224, "sigma(1670)+");
  urqmd2pdg[make_pair(58, -2)] = make_pair(23112, "sigma(1750)-");
  urqmd2pdg[make_pair(58, 0)] = make_pair(23212, "sigma(1750)0");
  urqmd2pdg[make_pair(58, 2)] = make_pair(23222, "sigma(1750)+");
  urqmd2pdg[make_pair(59, -2)] = make_pair(3116, "sigma(1775)-");
  urqmd2pdg[make_pair(59, 0)] = make_pair(3216, "sigma(1775)0");
  urqmd2pdg[make_pair(59, 2)] = make_pair(3226, "sigma(1775)+");
  urqmd2pdg[make_pair(60, -2)] = make_pair(13116, "sigma(1915)-");
  urqmd2pdg[make_pair(60, 0)] = make_pair(13216, "sigma(1915)0");
  urqmd2pdg[make_pair(60, 2)] = make_pair(13226, "sigma(1915)+");
  urqmd2pdg[make_pair(61, -2)] = make_pair(23114, "sigma(1940)-");
  urqmd2pdg[make_pair(61, 0)] = make_pair(23214, "sigma(1940)0");
  urqmd2pdg[make_pair(61, 2)] = make_pair(23224, "sigma(1940)+");
  urqmd2pdg[make_pair(62, -2)] = make_pair(3118, "sigma(2030)-");
  urqmd2pdg[make_pair(62, 0)] = make_pair(3218, "sigma(2030)0");
  urqmd2pdg[make_pair(62, 2)] = make_pair(3228, "sigma(2030)+");

  // Xis
  urqmd2pdg[make_pair(63, -1)] = make_pair(3312, "xi-");
  urqmd2pdg[make_pair(63, 1)] = make_pair(3322, "xi0");
  urqmd2pdg[make_pair(64, -1)] = make_pair(3314, "xi(1530)-");
  urqmd2pdg[make_pair(64, 1)] = make_pair(3324, "xi(1530)0");
  urqmd2pdg[make_pair(65, -1)] = make_pair(23314, "xi(1690)-");
  urqmd2pdg[make_pair(65, 1)] = make_pair(23324, "xi(1690)0");
  urqmd2pdg[make_pair(66, -1)] = make_pair(13314, "xi(1820)-");
  urqmd2pdg[make_pair(66, 1)] = make_pair(13324, "xi(1820)0");
  urqmd2pdg[make_pair(67, -1)] = make_pair(33314, "xi(1950)-");
  urqmd2pdg[make_pair(67, 1)] = make_pair(33324, "xi(1950)0");
  urqmd2pdg[make_pair(68, -1)] = make_pair(13316, "xi(2030)-");
  urqmd2pdg[make_pair(68, 1)] = make_pair(13326, "xi(2030)0");

  // Omega
  urqmd2pdg[make_pair(69, 0)] = make_pair(3334, "omega-");

  // Lambda_c
  urqmd2pdg[make_pair(70, 0)] = make_pair(4122, "lambda_c+");

  // Create the anti particles
  // urqmd_id -> -urqmd_id
  // iso3 -> -iso3
  // pdg_id -> -pdg_id
  // name -> anti_name
  for (auto const &[key, val] : urqmd2pdg) {
    int urqmd_id{key.first};
    int iso3{key.second};
    int pdg_id{val.first};
    std::string name{val.second};
    std::string new_name = "anti_" + name;
    urqmd2pdg[make_pair(-urqmd_id, -iso3)] = make_pair(-pdg_id, new_name);
  }


  // gamma
  urqmd2pdg[make_pair(100, 0)] = make_pair(22, "gamma");

  // pions
  urqmd2pdg[make_pair(101, -2)] = make_pair(-211, "pi-");
  urqmd2pdg[make_pair(101, 0)] = make_pair(111, "pi0");
  urqmd2pdg[make_pair(101, 2)] = make_pair(211, "pi+");

  // eta
  urqmd2pdg[make_pair(102, 0)] = make_pair(221, "eta");

  // omega(782)
  urqmd2pdg[make_pair(103, 0)] = make_pair(223, "omega");

  // rho(770)
  urqmd2pdg[make_pair(104, -2)] = make_pair(-213, "rho-");
  urqmd2pdg[make_pair(104, 0)] = make_pair(113, "rho0");
  urqmd2pdg[make_pair(104, 2)] = make_pair(213, "rho+");

  // f0(980) from comment in fortran file
  urqmd2pdg[make_pair(105, 0)] = make_pair(9010221, "f0(980)");

  // kaons
  urqmd2pdg[make_pair(106, -1)] = make_pair(311, "kaon0");
  urqmd2pdg[make_pair(-106, 1)] = make_pair(-311, "anti_kaon0");
  urqmd2pdg[make_pair(106, 1)] = make_pair(321, "kaon+");
  urqmd2pdg[make_pair(-106, -1)] = make_pair(-321, "kaon-");

  // eta prime(958)
  urqmd2pdg[make_pair(107, 0)] = make_pair(331, "eta_prime");

  // k*(892)
  urqmd2pdg[make_pair(108, -1)] = make_pair(313, "k_star0");
  urqmd2pdg[make_pair(-108, 1)] = make_pair(-313, "anti_k_star0");
  urqmd2pdg[make_pair(108, 1)] = make_pair(323, "k_star+");
  urqmd2pdg[make_pair(-108, -1)] = make_pair(-323, "k_star-");

  // phi(1020)
  urqmd2pdg[make_pair(109, 0)] = make_pair(333, "phi");

  // k0*(1430)
  urqmd2pdg[make_pair(110, -1)] = make_pair(10311, "k0_star(1430)0");
  urqmd2pdg[make_pair(-110, 1)] = make_pair(-10311, "anti_k0_star(1430)0");
  urqmd2pdg[make_pair(110, 1)] = make_pair(10321, "k0_star(1430)+");
  urqmd2pdg[make_pair(-110, -1)] = make_pair(-10321, "k0_star(1430)-");

  // a0(980) ????
  // comment in fortran file a0(980) which has PDG 9000111
  // pdg in file is 10211 which would be a0(1450)
  urqmd2pdg[make_pair(111, -2)] = make_pair(-10211, "a0(1450)-");
  urqmd2pdg[make_pair(111, 0)] = make_pair(10111, "a0(1450)0");
  urqmd2pdg[make_pair(111, 2)] = make_pair(10211, "a0(1450)+");

  // f0(1370)  wrong pdg in fortran file f0(1370) (PDG:30221) != 20221
  urqmd2pdg[make_pair(112, 0)] = make_pair(10221, "f0(1370)");

  // k1(1270)
  urqmd2pdg[make_pair(113, -1)] = make_pair(10313, "k1(1270)0");
  urqmd2pdg[make_pair(-113, 1)] = make_pair(-10313, "anti_k1(1270)0");
  urqmd2pdg[make_pair(113, 1)] = make_pair(10323, "k1(1270)+");
  urqmd2pdg[make_pair(-113, -1)] = make_pair(-10323, "k1(1270)-");

  // a1(1260)
  urqmd2pdg[make_pair(114, -2)] = make_pair(-20213, "a1(1260)-");
  urqmd2pdg[make_pair(114, 0)] = make_pair(20113, "a1(1260)0");
  urqmd2pdg[make_pair(114, 2)] = make_pair(20213, "a1(1260)+");

  // f1(1285)
  urqmd2pdg[make_pair(115, 0)] = make_pair(20223, "f1(1285)");

  // f1'(1510) wrong pdg in fortran file 40223
  urqmd2pdg[make_pair(116, 0)] = make_pair(9000223, "f1(1510)");

  // k2*(1430)
  urqmd2pdg[make_pair(117, -1)] = make_pair(315, "k2_star(1430)0");
  urqmd2pdg[make_pair(-117, 1)] = make_pair(-315, "anti_k2_star(1430)0");
  urqmd2pdg[make_pair(117, 1)] = make_pair(325, "k2_star(1430)+");
  urqmd2pdg[make_pair(-117, -1)] = make_pair(-325, "k2_star(1430)-");

  // a2(1329)
  urqmd2pdg[make_pair(118, -2)] = make_pair(-215, "a2(1320)-");
  urqmd2pdg[make_pair(118, 0)] = make_pair(115, "a2(1320)0");
  urqmd2pdg[make_pair(118, 2)] = make_pair(215, "a2(1320)+");

  // f2(1270)
  urqmd2pdg[make_pair(119, 0)] = make_pair(225, "f2(1270)");

  // f2'(1525)
  urqmd2pdg[make_pair(120, 0)] = make_pair(335, "f2_prime(1525)");

  // k1(1400)
  urqmd2pdg[make_pair(121, -1)] = make_pair(20313, "k1(1400)0");
  urqmd2pdg[make_pair(-121, 1)] = make_pair(-20313, "anti_k1(1400)0");
  urqmd2pdg[make_pair(121, 1)] = make_pair(20323, "k1(1400)+");
  urqmd2pdg[make_pair(-121, -1)] = make_pair(-20323, "k1(1400)-");

  // b1
  urqmd2pdg[make_pair(122, -2)] = make_pair(-10213, "b1(1235)-");
  urqmd2pdg[make_pair(122, 0)] = make_pair(10113, "b1(1235)0");
  urqmd2pdg[make_pair(122, 2)] = make_pair(10213, "b1(1235)+");

  // h1
  urqmd2pdg[make_pair(123, 0)] = make_pair(10223, "h1(1170)");

  // h1' not in fortran file if this is h1(1380) the pdg is 10333
  urqmd2pdg[make_pair(124, 0)] = make_pair(10333, "h1(1380)");

  // K* (1410) wrong pdgs in fortran file (30313, 30323)
  urqmd2pdg[make_pair(125, -1)] = make_pair(100313, "k_star(1410)0");
  urqmd2pdg[make_pair(-125, 1)] = make_pair(-100313, "anti_k_star(1410)0");
  urqmd2pdg[make_pair(125, 1)] = make_pair(100323, "k_star(1410)+");
  urqmd2pdg[make_pair(-125, -1)] = make_pair(-100323, "k_star(1410)-");

  // rho (1450) wrong pdgs in fortran file (-40213, 40113, 40213)
  urqmd2pdg[make_pair(126, -2)] = make_pair(-100213, "rho(1450)-");
  urqmd2pdg[make_pair(126, 0)] = make_pair(100113, "rho(1450)0");
  urqmd2pdg[make_pair(126, 2)] = make_pair(100213, "rho(1450)+");

  // omega (1420) wrong pdg in fortran file ( 50223)
  urqmd2pdg[make_pair(127, 0)] = make_pair(100223, "omega(1420)");

  // phi(1680) wrong pdg in fortran file ( 10223)
  urqmd2pdg[make_pair(128, 0)] = make_pair(100333, "phi(1680)");

  // k*(1680) wrong pdgs in fortran file ( 40313, 40323
  urqmd2pdg[make_pair(129, -1)] = make_pair(30313, "k_star(1680)0");
  urqmd2pdg[make_pair(-129, 1)] = make_pair(-30313, "anti_k_star(1680)0");
  urqmd2pdg[make_pair(129, 1)] = make_pair(30323, "k_star(1680)+");
  urqmd2pdg[make_pair(-129, -1)] = make_pair(-30323, "k_star(1680)-");

  // rho(1700)
  urqmd2pdg[make_pair(130, -2)] = make_pair(-30213, "rho(1700)-");
  urqmd2pdg[make_pair(130, 0)] = make_pair(30113, "rho(1700)0");
  urqmd2pdg[make_pair(130, 2)] = make_pair(30213, "rho(1700)+");

  // omega(1600) wrong pdg in fortran file ( 60223)
  urqmd2pdg[make_pair(131, 0)] = make_pair(30223, "omega(1650)");

  //  phi(1850)
  urqmd2pdg[make_pair(132, 0)] = make_pair(337, "phi(1850)");

  // D
  urqmd2pdg[make_pair(133, -1)] = make_pair(421, "D0");
  urqmd2pdg[make_pair(-133, 1)] = make_pair(-421, "anti_D0");
  urqmd2pdg[make_pair(133, 1)] = make_pair(411, "D+");
  urqmd2pdg[make_pair(-133, -1)] = make_pair(-411, "D-");

  // D*
  urqmd2pdg[make_pair(134, -1)] = make_pair(10421, "D*(2400)0");
  urqmd2pdg[make_pair(-134, 1)] = make_pair(-10421, "anti_D*(2400)0");
  urqmd2pdg[make_pair(134, 1)] = make_pair(10411, "D*(2400)+");
  urqmd2pdg[make_pair(-134, -1)] = make_pair(-10411, "D*(2400)-");

  // J/Psi
  urqmd2pdg[make_pair(135, 0)] = make_pair(443, "J/psi");

  // Chi_c
  urqmd2pdg[make_pair(136, 0)] = make_pair(10441, "Chi_c0");

  // Psi'
  urqmd2pdg[make_pair(137, 0)] = make_pair(100443, "Psi_prime");

  // Ds
  urqmd2pdg[make_pair(138, 0)] = make_pair(431, "Ds+");
  urqmd2pdg[make_pair(-138, 0)] = make_pair(-431, "Ds-");

  // Ds*
  urqmd2pdg[make_pair(139, 0)] = make_pair(433, "Ds*+");
  urqmd2pdg[make_pair(-139, 0)] = make_pair(-433, "Ds*-");

  // Deuteron
  urqmd2pdg[make_pair(200, 0)] = make_pair(1000010020, "Deuteron");
  urqmd2pdg[make_pair(-200, 0)] = make_pair(-1000010020, "anti_Deuteron");

  // Triton
  urqmd2pdg[make_pair(201, -1)] = make_pair(1000010030, "Triton");
  urqmd2pdg[make_pair(-201, 1)] = make_pair(-1000010030, "anti_Triton");

  // Helium3
  urqmd2pdg[make_pair(202, 1)] = make_pair(1000020030, "Helium3");
  urqmd2pdg[make_pair(-202, -1)] = make_pair(-1000020030, "anti_Helium3");

  // Helium4
  urqmd2pdg[make_pair(203, 0)] = make_pair(1000020040, "Helium4");
  urqmd2pdg[make_pair(-203, 0)] = make_pair(-1000020040, "anti_Helium4");

  // Hypertriton
  urqmd2pdg[make_pair(211, 0)] = make_pair(1010010030, "Hypertriton");
  urqmd2pdg[make_pair(-211, 0)] = make_pair(-1010010030, "anti_Hypertriton");

  // H^4_LAmbda ????
  urqmd2pdg[make_pair(213, -1)] = make_pair(1010010040, "Hyper???");
  urqmd2pdg[make_pair(-213, 1)] = make_pair(-1010010040, "anti_Hyper???");

  // H-dibaryon (Lambda-Lambda)
  urqmd2pdg[make_pair(220, 0)] = make_pair(1020000020, "H-dibaryon");
  urqmd2pdg[make_pair(-220, 0)] = make_pair(-1020000020, "anti_H-dibaryon");

  // Xi-dibaryon (Xi-N)
  // According to the PDG Monte Carlo Numbering Scheme the particle id is
  // the same as for the H-dibaryon. To distnguish the two particle the
  // Xi-dibarions misuses the number for the Isomer level even if it is not
  // an excited state of the H-dibaryon
  urqmd2pdg[make_pair(221, 0)] = make_pair(1020000021, "Xi-dibaryon");
  urqmd2pdg[make_pair(-221, 0)] = make_pair(-1020000021, "anti_Xi-dibaryon");

  // Xi-tribaryon (Xi-NN)
  urqmd2pdg[make_pair(222, 0)] = make_pair(1020000030, "Xi-tribaryon");
  urqmd2pdg[make_pair(-222, 0)] = make_pair(-1020000030, "anti_Xi-tribaryon");

/*
  for (auto const &[key, val] : urqmd2pdg) {
    int urqmd_id{key.first};
    int iso3{key.second};
    int pdg_id{val.first};
    std::string name{val.second};
    std::string new_name = "anti_" + name;
    std::cout << "UrQMD ID: " << key.first << "\n";
    std::cout << "ISO3    : " << key.second << "\n";
    std::cout << "PDG ID  : " << val.first << "\n";
    std::cout << "Name    : " << val.second << "\n";
    std::cout << "******************************\n";
  }
*/
  for (auto const &[key, val] : urqmd2pdg) {
     std::cout << std::setw(5) << key.first 
               << std::setw(5) << key.second 
               << std::setw(15) << val.first 
               << std::setw(25) << val.second << std::endl;
  }
  return 0;
}
