library(ggplot2)
library(ggpmisc)
library(ggthemes)
library(tidyverse)
library(dplyr)
library(plotly)
library(maps)
library(plotly)
library(janitor)
library(plyr)
library(lubridate)
library(forecast)
library(reshape)
library(reshape2)
library(viridis)
library(rddtools)
library(TTR)
library(ggfortify)
library(magrittr) 

#devtools::install_github("mdancho84/tidyquant")
library(tidyquant)  # Loads tidyverse, tidyquant, financial pkgs, xts/zoo
library(cranlogs)   # For inspecting package downloads over time
library(corrr)      # Tidy correlation tables and correlation plotting
library(cowplot) 


# If you want to update the data, set it as TRUE. 
# Or read csv directly from the link of the data
update_data <- FALSE

if (!dir.exists("data")) {
  dir.create("data")
}

if (update_data | !file.exists("data/owid-covid-data.csv")) {
  download.file("https://covid.ourworldindata.org/data/owid-covid-data.csv",
                destfile = file.path(getwd(), "/data/owid-covid-data.csv"))

}

if (update_data | !file.exists("data/OxCGRT_latest.csv")) {
  download.file("https://github.com/OxCGRT/covid-policy-tracker/blob/master/data/OxCGRT_latest.csv",
                destfile = file.path(getwd(), "/data/OxCGRT_latest.csv"))
  
}
if (update_data | !file.exists("data/time_series_covid19_recovered_global.csv")) {
  download.file("https://github.com/CSSEGISandData/COVID-19/blob/master/csse_covid_19_data/csse_covid_19_time_series/time_series_covid19_recovered_global.csv",
                destfile = file.path(getwd(), "/data/time_series_covid19_recovered_global.csv"))
  
}
covid_data <- read.csv("data/owid-covid-data.csv",
                       stringsAsFactors = FALSE,
                       check.names =  FALSE)

policy_data <- read.csv("data/OxCGRT_latest.csv",
                       stringsAsFactors = FALSE,
                       check.names =  FALSE)

recovery_data <- read.csv("data/time_series_covid19_recovered_global.csv",
                        stringsAsFactors = FALSE,
                        check.names =  FALSE)

df <- melt(recovery_data  %>% select(-`Province/State`,-Long,-Lat), id.vars = c("Country/Region"), variable.name = "date")
rsub <- aggregate(value~`Country/Region`+date,df,FUN=sum) %>% dplyr::rename(total_recovery=value)
rsub$date <- as.Date(as.character(rsub$date),"%Y%m%d")

world_map <- map_data("world2")
world_map <- world_map %>%
  mutate(region = replace(region, region == "UK","United Kingdom")) %>% 
  mutate(region = replace(region, region == "USA","United States")) 

covid_data$date <- as.Date(covid_data$date)
policy_data$Date <- as.Date(as.character(policy_data$Date),"%Y%m%d")
covid_data$new_cases[covid_data$new_cases < 0] <- 0
covid_data$new_cases_smoothed[covid_data$new_cases_smoothed < 0] <- 0
covid_data$new_cases[covid_data$new_deaths < 0] <- 0
covid_data$new_cases_smoothed[covid_data$new_deaths_smoothed < 0] <- 0
max_date<-max(covid_data$date)
min_date<-min(covid_data$date)
policy_not_state <-policy_data[policy_data$Jurisdiction != "STATE_TOTAL",]

policy_data$`C1_School closing` <- factor(policy_data$`C1_School closing`,
                    levels = c(-1, 0,1,2,3),
                    labels = c("No data","No measures","Recommended", "Required(at some levels)", "Required(at all levels)"))


policy_data$`C2_Workplace closing` <- factor(policy_data$`C2_Workplace closing`,
                                          levels = c(-1, 0,1,2,3), 
                                          labels = c("No data","No measures","Recommended", "Required(at some levels)", "Required(at all levels)"))

policy_data$`C3_Cancel public events` <- factor(policy_data$`C3_Cancel public events`,
                                          levels = c(-1, 0,1,2),
                                          labels = c("No data","No measures","Recommended", "Required"))
policy_data$`C4_Restrictions on gatherings` <- factor(policy_data$`C4_Restrictions on gatherings`,
                                          levels = c(-1, 0,1,2,3,4),
                                          labels = c("No data","No Restriction","Restriction on gathering > 1000", "Restriction on gathering 100~1000", "Restriction on gathering 10~100", "Restriction on gathering < 10"))

policy_data$`C5_Close public transport` <- factor(policy_data$`C5_Close public transport`,
                                                  levels = c(-1, 0,1,2),
                                                  labels = c("No data", "No measures", "Recommended closing", "Required closing"))


policy_data$`C6_Stay at home requirements` <- factor(policy_data$`C6_Stay at home requirements`,
                                                     levels = c(-1, 0,1,2,3),
                                                     labels = c("No data", "No measures", "Recommended", "Required (except essentials)", "Required (few exceptions)"))


policy_data$`C7_Restrictions on internal movement` <- factor(policy_data$`C7_Restrictions on internal movement`,
                                                             levels = c(-1, 0,1,2),
                                                             labels = c("No data", "No measures", "Recommended movement restriction", "Restrict movement"))


policy_data$`C8_International travel controls` <- factor(policy_data$`C8_International travel controls`,
                                                         levels = c(-1, 0,1,2,3,4),
                                                         labels = c("No data", "No restrictions", "Screening", "Quarantine from high-risk regions",
                                                                    "Ban on high-risk regions", "Total border closure"))


policy_data$`H1_Public information campaigns` <- factor(policy_data$`H1_Public information campaigns`,
                                                        levels = c(-1, 0,1,2),
                                                        labels = c("No data", "None", "Public officials urging caution", "Coordinated information campaign"))


policy_data$`H2_Testing policy` <- factor(policy_data$`H2_Testing policy`,
                                          levels = c(-1, 0,1,2,3),
                                          labels = c("No data", "No policy", "Symptoms & certain groups", "Anyone with symptoms", "Open public testing"))


policy_data$`H3_Contact tracing` <- factor(policy_data$`H3_Contact tracing`,
                                           levels = c(-1, 0,1,2),
                                           labels = c("No data", "No tracing", "Limited tracing(some cases)", "Comprehensive tracing(all cases)"))



policy_data$`H6_Facial Coverings` <- factor(policy_data$`H6_Facial Coverings`,
                                            levels = c(-1, 0,1,2,3,4),
                                            labels = c("No data", "No policy", "Recommended", "Required in some public places",
                                                       "Required in all public places", "Required outside the home at all times"))


policy_data$`H7_Vaccination policy` <- factor(policy_data$`H7_Vaccination policy`,
                                              levels = c(-1, 0,1,2,3,4,5),
                                              labels = c("No data", "None", "One group", "Two groups", "All vulnerable groups",
                                                         "Vulnerable & some others", "Universal availability"))


policy_data$`H8_Protection of elderly people` <- factor(policy_data$`H8_Protection of elderly people`,
                                                        levels = c(-1, 0,1,2,3),
                                                        labels = c("No data", "No measures", "Recommended isolation", "Narrow restrictions", "Extensive restrictions"))




