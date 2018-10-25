from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

zd_mass = '7'
tau0 = '1e-4'
job_label = 'mzd_' + zd_mass + '_tau0_' + tau0

config.General.requestName = 'Zprime_mu_pythia8_m0_' + zd_mass + 'GeV_tau0_' + tau0 + 'mm_pileup_gen-sim-raw_withPhaseSpace_10000'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'darkphoton_step2_pileup_cfg.py'
config.JobType.numCores = 4
config.JobType.maxMemoryMB = 10000

# same GEN-SIM dataset as no pileup
#config.Data.inputDataset = '/mzd_1_tau0_100/ufay-RunIISummer17PrePremix-MC_v2_94X_mc2017_realistic_v11_Pileup_full_GEN-SIM_v2-063effc73e0b2abdb9467bf954f836af/USER'
#config.Data.inputDBS = 'phys03'
config.Data.outputPrimaryDataset = job_label
config.Data.userInputFiles = open('mzd_7_tau0_1e-4mm_withPhaseSpace_gen-sim.txt').readlines()
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.outLFNDirBase = '/store/user/%s/Zprime_mu_pythia8/pileup/100000/' % (getUsernameFromSiteDB())
config.Data.publication = True
config.Data.outputDatasetTag = 'RunIISummer17PrePremix-MC_v2_94X_mc2017_realistic_v11_Pileup_full_GEN-SIM-RAW'

config.Site.storageSite = 'T2_US_Caltech'
config.Site.whitelist = ['T2_US_*']
