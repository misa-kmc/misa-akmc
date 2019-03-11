<a name="unreleased"></a>
## [Unreleased]


<a name="v0.1.0"></a>
## v0.1.0 - 2019-03-11
### Ci
- **coverage:** add gitlab-pages for showing test coverage report.
- **coverage:** add standard output of code coverage to get coverage report badges shown.
- **coverage:** add code test coverage configuration in .gitlab-ci.yml
- **gitlab-ci:** add gtest report for gitlab-ci.
- **gitlab-ci:** add test stage in .gitlab-ci.yml file.
- **gitlab-ci:** add gitlab ci configure file.

### Docs
- **comments:** add comments for header files.

### Feat
- add DEBUG_MODE macro  in src/building_config.in
- **atom:** add Atom and Itl class definition.
- **atomlist:** add a class to define AtomList data structure.
- **atomlist:** add fuctions in Class AtomList.
- **bonds:** add bonds energy table of different atoms pair.
- **bonds:** add feature of calculating bonds energy of a given lattice contributed by 1nn&2nn neighbour lattices.
- **box:** add feature of box containing lattice,ItlList,VacancyList.
- **counter:** add atoms and lattice types counter as well as its tests.
- **defect:** add rateCallback parameter to updateRates interface in class Defect to calculate transition rate.
- **direction:** add updateAvailTranDir for itl and vacancy defect.
- **eam:** add cmake option to use eam potential to calculate system energy.
- **energy:** add edumb function of inter energy contribution.
- **env:** add simulation environment.
- **inter:** add directions transformations of transition for inter lattices.
- **inter:** add ratesIndex for class Itl in src/defect/itl_list.h/.cpp
- **inter:** add feature of get 1nn neighbour id by rate index and feature of rates array status.
- **kmc:** add kmc event selecting.
- **kmc:** add feature of defect generation.
- **kmc:** make time increment as -ln(r)/total_rates, not 1/total_rates.
- **kmc:** add implementations of execution of KMC event (without recombination).
- **lattice-list:** add get methods by lattice id in class LatticesList.
- **lattice-list:** change array filling algorithm of get1nn and get2nn in LatticeList.
- **lattice-list:** add status of neighbour lattices(1nn, 2nn) for lattice list as well as its tests.
- **lattice-list:** add function get1nnBoundaryStatus and get2nnBoundaryStatus to class LatticesList.
- **lattice-type:** add class LatticeType as well as its unit tests.
- **lattice-type:** add member func of isHighEnd and isLowEnd for class LatticeTypes.
- **lattice-type:** add feature of getting atom type in high bits and low bits of dumbbell.
- **lattice-type:** add feature of getting diff of dumbbell and single atom.
- **lattice-type:** add feature of get first atom and second atom of dumbbell.
- **lattice_list:** add initialization for lattice types randomly, and combine atoms into inter lattice and vacancy lattice.
- **orientation:** add random function to generate orientation randomly.
- **plugin:** add plugin/interface of KMC event listener.
- **profile:** add system initializing and kmc simulation profile.
- **profile:** place solute atoms as dumbbells (not single atoms mixed with Fe).
- **profile:** add an example global_env and alloy ratio.
- **profile:** add lattice types counter for profile.
- **random:** add xoshiro rng algorithm.
- **random:** add multiple random number generation engines: LCG,MT,STC,xoshiro.
- **rate:** add class RatesUpdater which perform rates update for all lattices.
- **rate:** add Rate of each direction for class Itl in file itl_list.h and class Vacancy in file vacancy_list.h
- **rate:** calculate 4 transition rates and random a rotate angle, not 8 transition rates.
- **rate:** add rates solver interfce: class RatesSolver of file src/rate/rates_solver.h/.cpp
- **rates:** zero transition rates array in function beforeRatesUpdate for dummbell and vacancy trans
- **rates:** make kmc::updateRates returns sum of transition rates.
- **recombination:** add feature of recombination between interstitial and vacancy.

### Fix
- **$compile:** fix compile problem of "error: 'func_lattices_callback' is not a member of 'LatticesList' "
- **$compile:** add missing cassert header to fix compiling failure.
- **bonds:** remove contribution of dumbbell to transition atom.
- **inter:** fix compiler warning of "-Woverflow" in Itl::ratesIndex in src/defect/itl_list.cpp
- **lattice-list:** add 1nn and 2nn array initialization when using LatticesList::get1nn and get2nn f
- **rate:** change a param in RatesSolver::rate and relative functions from trans_atom to ghost_atom.
- **rate:** fix 'rate always be 0' problem by adding attempt frequency and temperature as constructor params in class RatesSolver.
- **recombination:** fix segment fault problem in 2nn traversing while creating recombination list.

### Refactor
- move class, rename member in class LatticesList, and fix some other words mistake.
- **atom:** removed atom coordinate in class Atom and add initialization of atom id.
- **atom:** rename type _type_atom_cord to _type_atom_coord and formated comments.
- **atomlist:** refactor class AtomList, including get1nn,get2nn with index boundary restrict an
- **atomlist:** change the type of parameters _1nn_list and _2nn_list in get1nn and get2nn from vector to array of Atoms pointer.
- **inter:** add beforeRatesUpdate and rename updateAvailTranDir to availTranDirs in class defect in defect/defect.hpp
- **kmc:** add more members for kmc event class SelectedEvent: to_id, target_tag, rotate directi
- **lattice-list:** rename class PureLatticeList to NormalLatticeList in src/lattice/normal_lattice.h
- **lattice-list:** make class LatticesList an interface for methods get1nn and get2nn.
- **orientation:** rename interval lattice "direction" to "orientation".
- **orientation:** use new orientation data structure: double-orientation.
- **rate:** change the params in RatesSolver constructor from Box& to LatticesList.
- **rate:** add e0() and deltaE() as member func in RatesSolver class and its derived class.
- **rate:** move updateRates method from RatesUpdater class in src/rates_updater.h to kmc class in kmc.h

### Test
- **atomlist:** add test for member AtomList::get1nn in class AtomList.
- **atomlist:** add test for member AtomList::get2nn in class AtomList.
- **bonds:** add missing bonds counting tests for Vacancy cases.
- **inter:** correct test for Itl::availTranDirs in tests/unit/itl_test.cpp
- **itl:** add tests for Itl::updateAvailTranDir() in file src/defect/itl_list.cpp
- **lattice-type:** add test for type combine and type diff.
- **rate:** correct tests for rates calculating of dumbbell transition.
- **rate:** correct the failed test of vacancy transition rate calculating.
- **rate:** correct failed tests in rates solver in vacancy transition.
- **rate:** add tests for rates solver of vacancy transition.

### BREAKING CHANGE

api of RatesSolver constructor, VacRatesSolver constructor and ItlRatesSolver
constructor has changed.

return value of get1nn and get2nn were changed.


[Unreleased]: https://git.hpcer.dev/HPCer/misa-kmc/kmc/compare/v0.1.0...HEAD
