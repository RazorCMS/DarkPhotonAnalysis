from CRABClient.UserUtilities import config, getUsernameFromSiteDB
config = config()

zd_mass = '7'
tau0 = '50'
job_label = 'mzd_' + zd_mass + '_tau0_' + tau0

config.General.requestName = 'Zprime_mu_pythia8_m0_' + zd_mass + 'GeV_tau0_' + tau0 + 'mm_pileup_AODSIM_withPhaseSpace_100000'
config.General.transferOutputs = True
config.General.transferLogs = False

config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'darkphoton_step3_cfg.py'
config.JobType.numCores = 4
config.JobType.maxMemoryMB = 10000 

#config.Data.inputDataset = '/mzd_7_tau0_100gmZmode_3_mass_check/ufay-RunIISummer17PrePremix-MC_v2_94X_mc2017_realistic_v11_Pileup_full_GEN-SIM-RAW_mass_check-424e4485a07f26f554e82f829d793003/USER'
#config.Data.inputDBS = 'phys03'
config.Data.outputPrimaryDataset = job_label
config.Data.userInputFiles = open('mzd_7_tau0_50mm_withPhaseSpace_gen-sim-raw.txt').readlines()
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.outLFNDirBase = '/store/user/%s/Zprime_mu_pythia8/pileup/100000' % (getUsernameFromSiteDB())
config.Data.publication = True
config.Data.outputDatasetTag = 'RunIISummer17PrePremix-MC_v2_94X_mc2017_realistic_v11_Pileup_AODSIM'

config.Site.storageSite = 'T2_US_Caltech'
config.Site.whitelist = ['T2_US_*']
