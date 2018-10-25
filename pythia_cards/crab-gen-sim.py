from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

zd_mass = '7'
tau0 = '50'
NJOBS = 1000
job_label = 'mzd_' + zd_mass + '_tau0_' + tau0

config.General.requestName = 'Zprime_mu_pythia8_m0_' + zd_mass + 'GeV_tau0_' + tau0 + 'mm_gen-sim_withPhaseSpace_100000'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'Zprime_pythia8_GEN-SIM_cfg.py'

config.Data.outputPrimaryDataset = job_label
config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 100
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.outLFNDirBase = '/store/user/%s/Zprime_mu_pythia8/GEN-SIM/100000' % (getUsernameFromSiteDB())
config.Data.publication = True
config.Data.outputDatasetTag = 'RunIISummer17PrePremix-MC_v2_94X_mc2017_realistic_v11_Pileup_full_GEN-SIM'

config.Site.storageSite = 'T2_US_Caltech'
config.Site.blacklist=['T2_RU_PNPI']
