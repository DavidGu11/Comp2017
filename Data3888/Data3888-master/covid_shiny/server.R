# Define server logic required to draw a histogram ----
server <- function(input, output) {
  
  # Plot of the new cases with selected countries ----
  # This expression that generates a plot is wrapped in a call
  # to renderPlot to indicate that:
  #
  # 1. It is "reactive" and therefore should be automatically
  #    re-executed when inputs (input$bins) change
  # 2. Its output type is a plot
  output$new_cases_plot <- renderPlot({
    countries <- input$countries
    
    ggplot(covid_data %>% filter(location == countries), 
           aes(x = date, y = new_cases, 
               group = location, color = location)) +
      geom_line(lwd = 1) +
      theme_bw() +
      ylab("Number of new cases") +
      scale_y_continuous(labels = scales::comma) +
      scale_x_date(date_breaks = "1 month") +
      theme(axis.text.x = element_text(angle = 90)) +
      labs(color = "Country/Region") +
      xlab("")
    
    
  })
  
  output$new_death_plot <- renderPlot({
    countries <- input$countries
    
    ggplot(covid_data %>% filter(location == countries), 
           aes(x = date, y = new_deaths, 
               group = location, color = location)) +
      geom_line(lwd = 1) +
      theme_bw() +
      ylab("Number of new deaths") +
      scale_y_continuous(labels = scales::comma) +
      scale_x_date(date_breaks = "1 month") +
      theme(axis.text.x = element_text(angle = 90)) +
      labs(color = "Country/Region") +
      xlab("")
    
  })
  
  output$new_cases_smooth_plot <- renderPlot({
    countries <- input$countries
    
    ggplot(covid_data %>% filter(location == countries), 
           aes(x = date, y = new_cases_smoothed, 
               group = location, color = location)) +
      geom_line(lwd = 1) +
      theme_bw() +
      ylab("Number of new cases") +
      scale_y_continuous(labels = scales::comma) +
      scale_x_date(date_breaks = "1 month") +
      theme(axis.text.x = element_text(angle = 90)) +
      labs(color = "Country/Region") +
      xlab("")
    
    
  })
  
  output$new_death_smooth_plot <- renderPlot({
    countries <- input$countries
    
    ggplot(covid_data %>% filter(location == countries), 
           aes(x = date, y = new_deaths_smoothed, 
               group = location, color = location)) +
      geom_line(lwd = 1) +
      theme_bw() +
      ylab("Number of new deaths") +
      scale_y_continuous(labels = scales::comma) +
      scale_x_date(date_breaks = "1 month") +
      theme(axis.text.x = element_text(angle = 90)) +
      labs(color = "Country/Region") +
      xlab("")
    
  })
  
  output$new_death_smooth_plot <- renderPlot({
    countries <- input$countries
    
    ggplot(covid_data %>% filter(location == countries), 
           aes(x = date, y = new_deaths_smoothed, 
               group = location, color = location)) +
      geom_line(lwd = 1) +
      theme_bw() +
      ylab("Government Stringency index (0~100)") +
      scale_y_continuous(labels = scales::comma) +
      scale_x_date(date_breaks = "1 month") +
      theme(axis.text.x = element_text(angle = 90)) +
      labs(color = "Country/Region") +
      xlab("")
    
  })
  
  output$gv_stringency_plot <- renderPlot({
    countries <- input$countries
    
    ggplot(covid_data %>% filter(location == countries), 
           aes(x = date, y = stringency_index, 
               group = location, color = location)) +
      geom_line(lwd = 1) +
      theme_bw() +
      ylab("Government Stringency index (0~100)") +
      scale_y_continuous(labels = scales::comma) +
      scale_x_date(date_breaks = "1 month") +
      theme(axis.text.x = element_text(angle = 90)) +
      labs(color = "Country/Region") +
      xlab("")
    
  })
  
  output$rdd_wd_plot <- renderPlot({
    countries <- input$loca
    data = covid_data %>% select("date","location", "reproduction_rate",
                          "new_cases_smoothed","new_deaths_smoothed", "stringency_index","new_vaccinations_smoothed")
    
    data   %>%
      select(stringency_index, reproduction_rate) %>%
      mutate(threshold = as.factor(ifelse(stringency_index >= 50, 1, 0))) %>%
      ggplot(aes(x = stringency_index, y = reproduction_rate, color = threshold)) +
      geom_point() +
      geom_smooth(method = "lm", se = FALSE) +
      scale_color_brewer(palette = "Accent") +
      guides(color = FALSE) +
      geom_vline(xintercept = 50, color = "red",
                 size = 1, linetype = "dashed") +
      labs(y = "reproduction rate",
           x = "stringency index",
           title = paste(countries, 'stringency VS reproduction rate'))
    
  })
  
  output$rdd_plot <- renderPlot({
    countries <- input$loca
    data = covid_data[covid_data$location == countries,
                      c("date","location", "reproduction_rate",
                        "new_cases_smoothed","new_deaths_smoothed", "stringency_index","new_vaccinations_smoothed")]
    
    data   %>%
      select(stringency_index, reproduction_rate) %>%
      mutate(threshold = as.factor(ifelse(stringency_index >= 50, 1, 0))) %>%
      ggplot(aes(x = stringency_index, y = reproduction_rate, color = threshold)) +
      geom_point() +
      geom_smooth(method = "lm", se = FALSE) +
      scale_color_brewer(palette = "Accent") +
      guides(color = FALSE) +
      geom_vline(xintercept = 50, color = "red",
                 size = 1, linetype = "dashed") +
      labs(y = "reproduction rate",
           x = "stringency index",
           title = paste(countries, 'stringency VS reproduction rate'))
    
  })
  
  output$map_total_case_plot <- renderPlot({
    specific_date <- input$dates
    
    options(scipen=999)
    covid_full_day <- covid_data %>% dplyr::filter(date == specific_date)
    world_map_with_data <- merge(world_map, covid_full_day, 
                                 by.x = "region", by.y = "location",
                                 all.x = TRUE)
    world_map_with_data <- world_map_with_data[order(world_map_with_data$order), ]
    breaks <- c(0, 10000, 10^c(5:8))
    
    world_map_with_data$total_cases[is.na(world_map_with_data$total_cases)] <- 0
    world_map_with_data$total_cases_category <- 
      cut(as.numeric(world_map_with_data$total_cases),
          breaks,include.lowest = TRUE, right = FALSE, dig.lab=10)
    reds_col <- RColorBrewer::brewer.pal(length(breaks) - 1, "Reds")
    names(reds_col) <- levels(world_map_with_data$total_cases_category)
    
    ggplot(world_map_with_data,  
           aes(x = long, y = lat, group = group, fill = total_cases_category)) +
      geom_polygon() +
      scale_fill_manual(values = reds_col) +
      xlab("") + ylab("") + ggtitle("Map of World") +
      theme_map() +
      theme(legend.position = "bottom", aspect.ratio = 0.6) +
      labs(title = paste('COVID19: Total number of cases'),
           subtitle = paste("Date:", specific_date), 
           fill = "")
    
  })
  
  output$total_deaths <- renderText({
    countries <- input$countries
    cases <- covid_data %>% filter(location == countries) %>% filter(date == max_date)
    print(as.numeric(cases$total_deaths))
  })
  
  output$population <- renderText({
    countries <- input$countries
    cases <- covid_data %>% filter(location == countries) %>% filter(date == max_date)
    print(as.numeric(cases$population))
  })
  
  output$total_cases <- renderText({
    countries <- input$countries
    cases <- covid_data %>% filter(location == countries) %>% filter(date == max_date)
    print(as.numeric(cases$total_cases))
  })
  
  output$compare_total_case_plot <- renderPlotly({
    countries <- input$countries
    p<-ggplot(data = covid_data %>% filter(location == countries), aes(x = date, y = reproduction_rate, group = 1,
                               text = paste("Date: ", date,
                                            "<br>Reproduction rate: ", reproduction_rate,
                                            "<br>Stringency index: ", stringency_index)
    )) + xlab('Date') + ylab('Reproduction rate')+
      scale_x_date(date_breaks = "1 month") + 
      theme(axis.text.x = element_text(angle = 90)) +
      geom_line(colour = "#408FA6")
    ggplotly(p)
    
  })
  
  output$forcast_vac_plot <- renderPlot({
    countries <- input$location
    location_data <- covid_data %>% filter(location == countries)
    inds <- seq(as.Date("2021-03-29"), as.Date("2022-03-01"), by = "days")
    timeseries= ts(location_data$total_vaccinations,start = c(2020,as.numeric(format(inds[1], "%j"))), frequency = 365)
    fit <- auto.arima(timeseries)
    autoplot(forecast(fit, h=30))

    # +  geom_line(
    #   aes(
    #     x = as.numeric(ts(location_data$total_vaccinations,start = c(2021,as.numeric(format(inds[1], "%j"))))),
    #     y = as.numeric()
    #   ),
    #   col = "red"
    # )
  })
  
  output$world_policy_plot <- renderPlotly({
    date <- input$Date
    policy_full_day <- policy_data %>% dplyr::filter(Date == date)
    policy_tab <- policy_full_day[policy_full_day$Jurisdiction != "STATE_TOTAL",]
    new_world_map <- merge(world_map, policy_tab,
                           by.x = "region", by.y = "CountryName",
                           all.x = TRUE)
    new_world_map <- new_world_map[order(new_world_map$order), ]
    if(input$policy=="School closure"){
      
      title = "School closures during COVID-19"
      title = paste(title, date)
      
     
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`C1_School closing`), region=region,text = paste('Region: ', region,
                                                                                                                                           '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                           '<br>School closing policy levels:', as.factor(`C1_School closing`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='School closing policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
      
    }else if(input$policy=="Workplace closure"){
      title = "Workplace closures during COVID-19"
      title = paste(title, date)
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`C2_Workplace closing`), region=region,text = paste('Region: ', region,
                                                                                                                                           '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                           '<br>Workplace closing policy levels:', as.factor(`C2_Workplace closing`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='Workplace closing policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
      
    }else if(input$policy=="Cancellaction of public events"){
      
      title = "Cancellaction of public events during COVID-19"
      title = paste(title, date)
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`C3_Cancel public events`), region=region,text = paste('Region: ', region,
                                                                                                                                           '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                           '<br>Cancellaction public events levels:', as.factor(`C3_Cancel public events`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='Cancellaction of public events policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
      
    }else if(input$policy=="Restrictions on gatherings"){
      policy_tab <- policy_full_day[policy_full_day$Jurisdiction != "STATE_TOTAL",]
      title = "Restrictions on gatherings during COVID-19"
      title = paste(title, date)
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`C4_Restrictions on gatherings`), region=region,text = paste('Region: ', region,
                                                                                                                                           '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                           '<br>Restrictions on gatherings policy levels:', as.factor(`C4_Restrictions on gatherings`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='Restrictions on gatherings policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
      
    }else if(input$policy=="Close public transport"){
      
      title = "Close public transport during COVID-19"
      title = paste(title, date)
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`C5_Close public transport`), region=region,text = paste('Region: ', region,
                                                                                                                                           '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                           '<br>Close public transport policy levels:', as.factor(`C5_Close public transport`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='Close public transport policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
    }else if(input$policy=="Stay at home requirements"){
      policy_tab <- policy_full_day[policy_full_day$Jurisdiction != "STATE_TOTAL",]
      title = "Stay at home requirements during COVID-19"
      title = paste(title, date)
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`C6_Stay at home requirements`), region=region,text = paste('Region: ', region,
                                                                                                                                           '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                           '<br>Stay at home requirements policy levels:', as.factor(`C6_Stay at home requirements`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='Stay at home requirements policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
    }else if(input$policy=="Restrictions on internal movement"){
      
      title = "Restrictions on internal movement during COVID-19"
      title = paste(title, date)
      
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`C7_Restrictions on internal movement`), region=region,text = paste('Region: ', region,
                                                                                                                                           '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                           '<br>Restrictions on internal movement policy levels:', as.factor(`C7_Restrictions on internal movement`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='Restrictions on internal movement policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
      
    }else if(input$policy=="International travel controls"){
      policy_tab <- policy_full_day[policy_full_day$Jurisdiction != "STATE_TOTAL",]
      title = "International travel controls during COVID-19"
      title = paste(title, date)
      
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`C8_International travel controls`), region=region,text = paste('Region: ', region,
                                                                                                                                                              '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                                              '<br>International travel controls policy levels:', as.factor(`C8_International travel controls`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='International travel controls policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
      
    }else if(input$policy=="Public information campaigns"){
      
      title = "Public information campaigns during COVID-19"
      title = paste(title, date)
      
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`H1_Public information campaigns`), region=region,text = paste('Region: ', region,
                                                                                                                                           '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                           '<br>Public information campaigns policy levels:', as.factor(`H1_Public information campaigns`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='Public information campaigns policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
      
    }else if(input$policy=="Testing policy"){
      policy_tab <- policy_full_day[policy_full_day$Jurisdiction != "STATE_TOTAL",]
      title = "Testing policy during COVID-19"
      title = paste(title, date)
      
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`H2_Testing policy`), region=region,text = paste('Region: ', region,
                                                                                                                                            '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                            '<br>Testing policy levels:', as.factor(`H2_Testing policy`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='Testing policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
      
    }else if(input$policy=="Contact tracing"){
      
      title = "Contact tracing during COVID-19"
      title = paste(title, date)
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`H3_Contact tracing`), region=region,text = paste('Region: ', region,
                                                                                                                                                              '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                                              '<br>Contact tracing policy levels:', as.factor(`H3_Contact tracing`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='Contact tracing policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
      
    }else if(input$policy=="Facial coverings"){
      policy_tab <- policy_full_day[policy_full_day$Jurisdiction != "STATE_TOTAL",]
      title = "Facial coverings during COVID-19"
      title = paste(title, date)
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`H6_Facial Coverings`), region=region,text = paste('Region: ', region,
                                                                                                                                                              '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                                              '<br>Facial coverings policy levels:', as.factor(`H6_Facial Coverings`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='Facial coverings policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
      
    }else if(input$policy=="Vaccination policy"){
      
      title = "Vaccination policy during COVID-19"
      title = paste(title, date)
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`H7_Vaccination policy`), region=region,text = paste('Region: ', region,
                                                                                                                                                              '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                                              '<br>Vaccination policy levels:', as.factor(`H7_Vaccination policy`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='Vaccination policy policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
      

    }else if(input$policy=="Protection of elderly people"){
      policy_tab <- policy_full_day[policy_full_day$Jurisdiction != "STATE_TOTAL",]
      title = "Protection of elderly people during COVID-19"
      title = paste(title, date)
      
      school_plot <- ggplot(new_world_map, aes(x=long, y=lat, group=group, fill=as.factor(`H8_Protection of elderly people`), region=region,text = paste('Region: ', region,
                                                                                                                                                              '<br>Stringency Index:', StringencyIndex, 
                                                                                                                                                              '<br>Protection of elderly people policy levels:', as.factor(`H8_Protection of elderly people`)
      ))) +
        geom_polygon() + theme_void() + xlab("") + ylab("") + ggtitle(title) + labs(fill='Protection of elderly people policy levels') 
      theme(panel.grid = element_blank(), axis.ticks.x = element_blank(), axis.ticks.y = element_blank(),
            legend.title = element_blank())
      ggplotly(school_plot, tooltip=c("text"))
    }

  })
  
  
  output$local_policy_plot <- renderPlotly({
    country <- input$country
    if(input$policies=="School closure"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country) %>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `C1_School closing`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: School closing policy level", `C1_School closing`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)

    }else if(input$policies=="Workplace closure"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `C2_Workplace closing`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: Workplace closing policy level", `C2_Workplace closing`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)

    }else if(input$policies=="Cancellaction of public events"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `C3_Cancel public events`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: Cancel public events policy level", `C3_Cancel public events`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)
    }else if(input$policies=="Restrictions on gatherings"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `C4_Restrictions on gatherings`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: Restrictions on gatherings policy level", `C4_Restrictions on gatherings`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)
    
    }else if(input$policies=="Close public transport"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `C5_Close public transport`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: Close public transport policy level", `C5_Close public transport`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)
      
    }else if(input$policies=="Stay at home requirements"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `C6_Stay at home requirements`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: Stay at home requirements policy level", `C6_Stay at home requirements`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)
     
    }else if(input$policies=="Restrictions on internal movement"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `C7_Restrictions on internal movement`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: Restrictions on internal movement policy level", `C7_Restrictions on internal movement`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)

    }else if(input$policies=="International travel controls"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `C8_International travel controls`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: International travel controls policy level", `C8_International travel controls`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)

    }else if(input$policies=="Public information campaigns"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `H1_Public information campaigns`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: Public information campaigns policy level", `H1_Public information campaigns`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)

    }else if(input$policies=="Testing policy"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `H2_Testing policy`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: Testing policy level", `H2_Testing policy`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)
    }else if(input$policies=="Contact tracing"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `H3_Contact tracing`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: Contact tracing policy level", `H3_Contact tracing`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)
      
    }else if(input$policies=="Facial coverings"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `H6_Facial Coverings`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: Facial coverings policy level", `H6_Facial Coverings`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)

    }else if(input$policies=="Vaccination policy"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `H7_Vaccination policy`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: Vaccination policy level", `H7_Vaccination policy`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)

    }else if(input$policies=="Protection of elderly people"){
      p<-ggplot(data = policy_data %>% filter(CountryName == country)%>% filter(Jurisdiction != "STATE_TOTAL"), aes(x = Date, y = `H8_Protection of elderly people`, group=1,
                                                                           text = paste("Date: ", Date,
                                                                                        "<br>: Protection of elderly people policy level", `H8_Protection of elderly people`,
                                                                                        "<br>Stringency index: ", StringencyIndex)
      )) + xlab('Date') + ylab('Stringency level')+
        scale_x_date(date_breaks = "1 month") +
        theme(axis.text.x = element_text(angle = 90)) +
        geom_line(colour = "#408FA6")
      ggplotly(p)
      
    }
  
  })
  
  output$rolling_vac_plot <- renderPlotly({
    # Get rolling correlations\
    country = input$location
    new <- covid_data  %>% filter(location==country) %>%
      select(date, location, new_vaccinations, new_cases)
    new <- na.omit(new)
    
    # psub <- policy_data%>% filter(Jurisdiction != "STATE_TOTAL") %>% filter(CountryName==country)
    # new_policy <- psub[c("Date","CountryName","H7_Vaccination policy")]
    # cnew <- merge(x = new_policy, y = new, x.by="Date", y.by = "date", all.x = TRUE)
    rolling_corr <- new %>%
      # Mutation
      tq_mutate_xy(
        x          = new_vaccinations,
        y          = new_cases,
        mutate_fun = runCor, 
        # runCor args
        n          = 30,
        use        = "pairwise.complete.obs",
        # tq_mutate args
        col_rename = "rolling_corr"
      )
   #  new_cases <- new[,c("new_cases")]
   #  new_vaccination <- new[,c("new_vaccinations")]
   static_corr <- new %>%
      select(-date,-location) %>%
      correlate()
   res<-na.omit(static_corr$new_vaccinations)
   rolling_corr[["static_corr"]]<-res[1]

    
    p<-rolling_corr %>%
      ggplot(aes(x = date, color = location)) +
      # Data
      geom_point(aes(y = rolling_corr), alpha = 0.5) +
      geom_line(aes(y = static_corr), color = "red") +
      facet_wrap(~ location, ncol = 3, scales = "free_y") +
      
      # Aesthetics
      scale_color_tq() +
      labs(
        title = "1 month Rolling Correlation between new vaccination and new cases",
        subtitle = "Relationships are dynamic vs static correlation (red line)",
        x = "", y = "Correlation"
      ) +
      theme_tq() +
      
      scale_x_date(date_breaks = "1 month") +
      theme(axis.text.x = element_text(angle = 90)) +
      theme(legend.position="none")
    ggplotly(p) 
    # %>% layout(title = list(text = paste0('Rolling Correlation between new vaccination and new cases',
    #                                               '<br>',
    #                                               '<sup>',
    #                                               'Relationships are dynamic vs static correlation (red line)','</sup>')))
    
  }
  )
  
  output$correlation_ari <- renderPlotly({
    
  })
  
  output$correlation_ets <- renderPlotly({
    
  })
  
  output$xgboost_plot <- renderPlot({

    
  })
  
  
  
}



