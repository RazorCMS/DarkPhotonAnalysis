import os,sys,re,fileinput,string,shutil
import numpy as np
import ROOT as rt
import glob

from array import array
rt.gROOT.SetBatch(True)

inputfname=str(sys.argv[1])
chain = rt.TChain('tree')
with open(inputfname) as inputfile:
    for line in inputfile:
        line=line.rstrip()
        print line.rstrip()
        chain.Add(line)
print chain.GetEntries()
#chain.Draw("mass","","p")
#c.SaveAs("mass.pdf")
#c.SaveAs("mass.png")
mass = array( 'f', [ 0.] )
chain.SetBranchAddress("mass",mass)
h_mass_all = rt.TH1D("mass_all","mass",56000,0.,140.)
h_mass_1p5 = rt.TH1D("mass_1p5","mass",600,0.,1.5)
h_mass_1p5To3p5 = rt.TH1D("mass_1p5To3p5","mass",800,1.5,3.5)
h_mass_3p3To5p5 = rt.TH1D("mass_3p3To5p5","mass",880,3.3,5.5)
h_mass_1p5To5p5 = rt.TH1D("mass_1p5To5p5","mass",1600,1.5,5.5)
h_mass_8To11= rt.TH1D("mass_8To11","mass",1200,8.,11.)
for i in range(0,chain.GetEntries()):
    chain.GetEvent(i)
    h_mass_all.Fill(mass[0])
    h_mass_1p5.Fill(mass[0])
    h_mass_1p5To3p5.Fill(mass[0])
    h_mass_3p3To5p5.Fill(mass[0])
    h_mass_1p5To5p5.Fill(mass[0])
    h_mass_8To11.Fill(mass[0])
name=str(sys.argv[2])
ofile = rt.TFile.Open(name,"RECREATE"); 
#chain.CloneTree(-1,"fast") 
#chain.Draw("mass>>h(1000,0.,100.)")
h_mass_all.Write()
h_mass_8To11.Write()
h_mass_1p5.Write()
h_mass_1p5To3p5.Write()
h_mass_3p3To5p5.Write()
h_mass_1p5To5p5.Write()
ofile.cd()
ofile.Write()
ofile.Close()
