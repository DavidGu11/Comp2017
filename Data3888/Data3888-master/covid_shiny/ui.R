library(shiny)
library(shinyWidgets)
library(shinydashboard)
library(leaflet.minicharts)
library(leaflet)
library(crosstalk)


# Define UI for app that draws a histogram ----
ui <- dashboardPage(
  dashboardHeader(title="CovidP9"),
  dashboardSidebar(
    sidebarMenu(
    menuItem("Introduction", tabName = "introduction", icon = icon("sitemap")),
    menuItem("Questions", tabName = "widgets", icon = icon("align-justify"),
             menuSubItem("Policy Overview", tabName = "q1", icon = icon("question")),
             menuSubItem("Covid Overview", tabName = "q2", icon = icon("question")),
             menuSubItem("Impact of policy implied", tabName = "q4", icon = icon("question")),
             menuSubItem("Vaccination", tabName = "q3", icon = icon("question"))
             ),
             
    menuItem("Help", tabName = "profile", icon = icon("users")))
    
  ), 
  
  dashboardBody(
    tabItems(
      tabItem(tabName = "introduction", 
              mainPanel(
                h1("Impact of government policy response"),
                img(src='policy.png', height="50%", width="50%",align = "right"),
                h2("Background"),
                h4("In March 2020, the World Health Organization (WHO) declared COVID-19 a pandemic, caused by the novel SARS-CoV-2 virus, since then, the study of its impact in every aspect of the world has always been one amongst the foremost scientifically and technologically difficult issues round the world."),
                h2("Aim"),
                h4(" Our study concentrates on discovering how different levels of government responses have contributed to affect the spread of COVID-19. The process of the analysis will be done using innovative approaches such as regression discontinuity analysis to add insight into experimental study of the global pendemic. The study also aims to locate an appropriate forecasting method to predict the correlation of vaccination and cases to look into how effectivness of the vaccines will change over time."),
                h2("Dataset"),
                h4("The owid-covid-data dataset is a collection of the COVID-19 data maintained by Our World in Data. Meanwhile, the Oxford Covid-19 Government Response Tracker(OxCGRT) collects systematic information on policy measures that have been utilised by governments to address the challenge of Covid-19. The dataset is collected by the Blavatnik School of Government. Our investigation of the dataset majorly involves Total cases/Total deaths(Total confirmed cases/deaths of COVID-19) and Total vaccinations(Total number of COVID-19 vaccination doses administered) from the owid-covid-data dataset."),
                width = 9
              )
          ),
      tabItem(tabName = "q2",
        sidebarLayout(
          
          # Sidebar panel for inputs ----
          
          
          sidebarPanel(
            selectInput("countries", "Countries/Regions:",
                        choices = unique(covid_data$location),
                        selected = "Australia")
            ),
          
          # Main panel for displaying outputs ----
          mainPanel(
            fluidRow(
              box(
                title = "Total Deaths", width = 4, background = "green",
                textOutput("total_deaths")
              ),
              box(
                title = "Total cases",width = 4, background = "maroon",
                textOutput("total_cases")
              ),
              box(
                title = "Population", width = 4, background = "yellow",
                textOutput("population")
              )
            ),
            tabsetPanel(
              id="con",
              tabPanel("New cases", value=1,plotOutput(outputId = "new_cases_plot")), 
              tabPanel("New cases Smoothed", value=1,plotOutput(outputId = "new_cases_smooth_plot")),
              tabPanel("New deaths", value=1,plotOutput(outputId = "new_death_plot")),
              tabPanel("New deaths smoothed",value=1, plotOutput(outputId = "new_death_smooth_plot")),
              tabPanel("Government Stringency Index",value=1, plotOutput(outputId = "gv_stringency_plot")),
              tabPanel("Reproducation Rate", value=1,plotlyOutput(outputId = "compare_total_case_plot"))
            )
          )
        )
      ),
      
    tabItem(tabName="q1",
       sidebarLayout(
         sidebarPanel(
           conditionalPanel(condition="input.public==1",
                            h3("Policy Response:"),
                            tags$br(),
                            pickerInput(
                              "policy",
                              "Select policy:",
                              choices=c("School closure",
                                        "Workplace closure", "Cancellaction of public events",
                                        "Restrictions on gatherings", "Close public transport",
                                        "Stay at home requirements", "Restrictions on internal movement", "International travel controls",
                                        "Public information campaigns", "Contact tracing", "Testing policy",
                                        "Facial coverings", "Vaccination policy", "Protection of elderly people")
                            ),
                            dateInput("Date", "Select date:",
                                      value = max_date,
                                      max = max_date)),
           
           conditionalPanel(condition="input.public==2",
                            h3("Choose country:"),
                            tags$br(),
                            pickerInput(
                              "country",
                              "Select country:",
                              choices = unique(policy_not_state$CountryName)
                            ),
                            pickerInput(
                              "policies",
                              "Select policy:",
                              choices=c("School closure",
                                        "Workplace closure", "Cancellaction of public events",
                                        "Restrictions on gatherings", "Close public transport",
                                        "Stay at home requirements", "Restrictions on internal movement", "International travel controls",
                                        "Public information campaigns", "Contact tracing", "Testing policy",
                                        "Facial coverings", "Vaccination policy", "Protection of elderly people")
                            ))
                          
           ),

         mainPanel(
           tabsetPanel(
             type = "tabs",
             tabPanel("World Policy Response", value=1,
                      plotlyOutput(outputId = "world_policy_plot")),
             tabPanel("Location policy Response", value=2,
                      plotlyOutput(outputId = "local_policy_plot")),
             id="public")
         )
       )
    ),
    
      
      
      
      tabItem(tabName = "q3",
              sidebarPanel(
                selectInput("location", "Countries/Regions:",
                            choices = unique(covid_data$location),
                            selected = "Australia")
              ),
              
              # Main panel for displaying outputs ----
              mainPanel(
                tabsetPanel(
                  # tabPanel("Turning point detection",plotlyOutput(outputId = "turning_point_plot")),
                  tabPanel("Rolling correlations",plotlyOutput(outputId = "rolling_vac_plot")), 
                  tabPanel("Vaccination forecast",plotOutput(outputId = "forcast_vac_plot"))
                  
                )
              )
              
              
        # sidebarLayout(
        #   
        #   sliderInput(
        #     inputId = 'dates', label = 'Date', 
        #     min = as.Date("2020-01-01","%Y-%m-%d"),
        #     max = as.Date("2022-05-01","%Y-%m-%d"),
        #     value=as.Date("2021-01-01"),
        #     ticks = FALSE,
        #     timeFormat = "%Y-%m-%d",
        #     step = 1, width = '95%',
        #     animate = TRUE, animateOptions(interval = 8000, loop = TRUE)
        #   ),
        # 
        #   
        #   # Main panel for displaying outputs ----
        #   mainPanel(
        #     tabsetPanel(
        #       tabPanel("Map", plotOutput(outputId = "map_total_case_plot"))
        #       
        #     )
        #   )
        # )
      ),
    
    tabItem(tabName = "q4",
            sidebarPanel(
              selectInput("loca", "Countries/Regions:",
                          choices = unique(covid_data$location),
                          selected = "Australia")
              # selectInput("loca1", "DID Countries/Regions1:",
              #             choices = unique(covid_data$location),
              #             selected = "Australia"),
              # selectInput("loca2", "DID Countries/Regions2:",
              #             choices = unique(covid_data$location),
              #             selected = "Australia"),
              # dateInput("date", "Select date:",
              #           value = max_date,
              #           max = max_date)
              
            ),

            # Main panel for displaying outputs ----
            mainPanel(
              h3("Rdd Analysis between Stringency index v.s. reproduction rate"),
              tabsetPanel(
                tabPanel("World",plotOutput(outputId = "rdd_wd_plot")),
                tabPanel("Country",plotOutput(outputId = "rdd_plot"))
              )
              # h3("DID Analysis between Stringency index v.s. reproduction rate"),
              # tabsetPanel(
              #   tabPanel("world",plotOutput(outputId = "did_wd_plot")),
              #   tabPanel("Country",plotOutput(outputId = "did_plot")),
              # ),
            )
          ),
    tabItem(tabName = "profile", 
            mainPanel(
              fluidRow(
                box(width = 12, status="info",
                    HTML("
                <h3 style = 'font-weight:bold'> How to use</h3>
                <h4 style = 'font-weight:bold'> Policy Overview</h3>
                <p style = 'font-size:18px;font-weight:normal'>
                  In policy Overview, you can select the specific policy types to view the word trends or location timeline to check the policy level changes over times. 
            </p>
            <h4 style = 'font-weight:bold'> Covid Overview</h3>
            <p style = 'font-size:18px;font-weight:normal'>
                In Covid Overview, user can select the location to check with the new cases, new deaths, stringency index, and reproduction rate. The trends of these features was clearly shown. 
            </p>
            
            <h4 style = 'font-weight:bold'> RDD</h3>
            <p style = 'font-size:18px;font-weight:normal'>
                RDD is a quasi-experimental evaluation method that is widely used in time series analysis. The method features applying a treatment assignment mechanism based on a continuous eligibility index(a variable with a continuous distribution) to measure the casual effects of an intervention. In application, it finds a cutoff point to identify the impact. 
            </p>
            <p style = 'font-size:18px;font-weight:normal'>
                Our regression discontinuity design has satisfied the following conditions to become a appropriate model in measuring the effect of stringency index:
                
                1.A continuously eligibility index, which, in our case is the stringency index.
                
                2.A clearly defined cutoff point, which, in our case is 50 where the level of government involvement is deemed as somewhat moderate.
                
                Our regression discontinuity design is developed based on the following assumptions:
                
                1. The stringency index should be continuous around the cutoff point. The stringency index is not being intentionally manipulated by governments to create a pretended effort in controlling the spread of COVID-19. 
                
                2. Countries close to the cutoff point should be somewhat similar.
                
                The cutoff point at 50 divides the level of government involvement in controlling COVID-19 to mild and strict.             </p>
            <h4 style = 'font-weight:bold'> Vaccination</h3>
            <p style = 'font-size:18px;font-weight:normal'>
            The rolling correlation shows the rolling nature of new cases and new vaccinations, which calculated under 30 days sample size. The forecast section shows the arima forecase result of next 30 days
            </p>
            <h4 style = 'font-weight:bold'> Group Members</h3>

            <br> 
                Haoran Guan, Jessica Han, Jingxuan Wang, Yuxuan Wang, Ziwen Gu
                Our GitHub Links: 
                <a href = https://github.sydney.edu.au/zigu7311/Data3888>
                https://github.sydney.edu.au/zigu7311/Data3888
                </a>
            <br>
            </p>
            <br>")),)
              
            )
      )
    )
  )
)
